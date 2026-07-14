#include <cstdint>
#include <format>
#include <iostream>
#include <span>
#include <string>
#include <string_view>

#include "cpuid.hpp"
#include "msr.hpp"
#include "printer.hpp"
#include "printer/json.hpp"
#include "vmx/ia32_vmx_basic.hpp"
#include "vmx/ia32_vmx_pinbased_ctls.hpp"
#include "vmx/ia32_vmx_procbased_ctls.hpp"
#include "vmx/ia32_vmx_procbased_ctls2.hpp"
#include "vmx/ia32_vmx_exit_ctls.hpp"
#include "vmx/ia32_vmx_entry_ctls.hpp"
#include "vmx/ia32_vmx_ept_vpid_cap.hpp"

namespace
{
    constexpr uint32_t ia32_vmx_basic = 0x480;
    constexpr uint32_t ia32_vmx_pinbased_ctls = 0x481;
    constexpr uint32_t ia32_vmx_procbased_ctls = 0x482;
    constexpr uint32_t ia32_vmx_procbased_ctls2 = 0x48b;
    constexpr uint32_t ia32_vmx_exit_ctls = 0x483;
    constexpr uint32_t ia32_vmx_entry_ctls = 0x484;
    constexpr uint32_t ia32_vmx_ept_vpid_cap = 0x48c;

    std::string_view msr_status_hint(vmx::msr::msr_status s)
    {
        switch (s)
        {
        case vmx::msr::msr_status::not_applicable:
            return "vmx not supported - no vmx capability msrs on this cpu";
        case vmx::msr::msr_status::permission_denied:
            return "msr read unavailable - run with sudo (linux) or install driver (windows)";
        case vmx::msr::msr_status::not_supported:
            return "msr read unavailable - /dev/cpu/N/msr missing (load the msr module)";
        case vmx::msr::msr_status::not_implemented:
            return "msr read unavailable - no msr backend on this platform";
        case vmx::msr::msr_status::io_error:
            return "msr read unavailable - i/o error";
        case vmx::msr::msr_status::ok:
            return {};
        }
        return {};
    }

    enum class output_mode
    {
        text,
        json
    };

    struct cli_options
    {
        output_mode mode = output_mode::text;
        bool no_color = false;
        bool help = false;
        std::string_view unknown{};
    };

    cli_options parse_args(std::span<char*> args)
    {
        cli_options opts{};
        for (size_t i = 1; i < args.size(); ++i)
        {
            const std::string_view flag{args[i]};
            if (flag == "--no-color")
            {
                opts.no_color = true;
            }
            else if (flag == "--json")
            {
                opts.mode = output_mode::json;
            }
            else if (flag == "--help")
            {
                opts.help = true;
            }
            else
            {
                opts.unknown = flag;
            }
        }
        return opts;
    }

    void print_usage(std::ostream & out)
    {
        out << "usage: vmx-check [--json] [--no-color] [--help]\n";
    }

    void print_help(std::ostream & out)
    {
        out << "vmx-check - intel vt-x capability dumper\n\n";
        print_usage(out);
        out << "\n"
               "flags:\n"
               "    --json         emit json instead of text\n"
               "    --no-color     disable ansi colors\n"
               "    --help         print this help\n";
    }

    struct cpu_signature
    {
        uint32_t family;
        uint32_t model;
        uint32_t stepping;
    };

    // sdm vol.2a, cpuid eax=01h, version information in eax
    cpu_signature decode_signature(uint32_t eax)
    {
        const uint32_t stepping = eax & 0xf;
        const uint32_t base_model = (eax >> 4) & 0xf;
        const uint32_t base_family = (eax >> 8) & 0xf;
        const uint32_t extended_model = (eax >> 16) & 0xf;
        const uint32_t extended_family = (eax >> 20) & 0xff;

        cpu_signature sig{base_family, base_model, stepping};
        if (base_family == 0x0f) sig.family += extended_family;
        if (base_family == 0x06 || base_family == 0x0f) sig.model += extended_model << 4;
        return sig;
    }

    std::string printable(std::string_view value)
    {
        std::string out{};
        for (const char c : value)
        {
            const auto byte = static_cast<unsigned char>(c);
            out.push_back(byte < 0x20 || byte >= 0x7f ? '?' : c);
        }
        return out;
    }

    std::string decode_string(std::initializer_list<uint32_t> regs)
    {
        std::string text{};
        for (const auto reg : regs)
        {
            for (int shift = 0; shift < 32; shift += 8)
            {
                text.push_back(static_cast<char>(reg >> shift));
            }
        }
        return text;
    }

    std::string cpu_vendor()
    {
        const auto regs = vmx::read_cpuid(0);
        return decode_string({regs.ebx, regs.edx, regs.ecx});
    }

    std::string hypervisor_vendor()
    {
        const auto regs = vmx::read_cpuid(0x40000000);
        std::string vendor = decode_string({regs.ebx, regs.ecx, regs.edx});
        vendor.erase(vendor.find_last_not_of('\0') + 1);
        return vendor;
    }

    struct report
    {
        std::string vendor;
        cpu_signature sig;
        bool vmx_supported;
        bool is_amd;
        bool svm_supported;
        bool hypervisor_present;
        std::string hypervisor_id;
        vmx::msr::msr_status msr_status;
        bool have_msr;
        vmx::caps::ia32_vmx_basic_fields basic;
        bool have_pinbased;
        vmx::caps::ia32_vmx_pinbased_ctls_fields pinbased;
        bool have_procbased;
        vmx::caps::ia32_vmx_procbased_ctls_fields procbased;
        bool have_procbased2;
        vmx::caps::ia32_vmx_procbased_ctls2_fields procbased2;
        bool have_exit;
        vmx::caps::ia32_vmx_exit_ctls_fields exit_ctls;
        bool have_entry;
        vmx::caps::ia32_vmx_entry_ctls_fields entry_ctls;
        bool have_ept_vpid;
        vmx::caps::ia32_vmx_ept_vpid_cap_fields ept_vpid;
    };

    report gather()
    {
        report r{};
        r.vendor = cpu_vendor();

        const auto leaf1 = vmx::read_cpuid(1);
        r.sig = decode_signature(leaf1.eax);

        // sdm vol.3 24.6, cpuid eax=01h, ecx bit 5
        constexpr uint32_t vmx_feature_bit = 1u << 5;
        r.vmx_supported = (leaf1.ecx & vmx_feature_bit) != 0;

        r.is_amd = r.vendor == "AuthenticAMD";
        if (r.is_amd)
        {
            // amd apm vol.3, cpuid fn8000_0001h, ecx bit 2
            constexpr uint32_t svm_feature_bit = 1u << 2;
            r.svm_supported = (vmx::read_cpuid(0x80000001).ecx & svm_feature_bit) != 0;
        }

        // cpuid eax=01h ecx bit 31; vendor id in leaf 40000000h ebx, ecx, edx
        constexpr uint32_t hypervisor_present_bit = 1u << 31;
        r.hypervisor_present = (leaf1.ecx & hypervisor_present_bit) != 0;
        if (r.hypervisor_present) r.hypervisor_id = hypervisor_vendor();

        if (!r.vmx_supported)
        {
            r.msr_status = vmx::msr::msr_status::not_applicable;
            return r;
        }

        const auto probe = vmx::msr::read_msr(ia32_vmx_basic);
        r.msr_status = probe.status;
        r.have_msr = probe.status == vmx::msr::msr_status::ok;
        if (!r.have_msr) return r;
        r.basic = vmx::caps::parse_basic(probe.value);

        const auto pinbased = vmx::msr::read_msr(ia32_vmx_pinbased_ctls);
        r.have_pinbased = pinbased.status == vmx::msr::msr_status::ok;
        if (r.have_pinbased) r.pinbased = vmx::caps::parse_pinbased_ctls(pinbased.value);

        const auto procbased = vmx::msr::read_msr(ia32_vmx_procbased_ctls);
        r.have_procbased = procbased.status == vmx::msr::msr_status::ok;
        if (r.have_procbased) r.procbased = vmx::caps::parse_procbased_ctls(procbased.value);

        const auto procbased2 = vmx::msr::read_msr(ia32_vmx_procbased_ctls2);
        r.have_procbased2 = procbased2.status == vmx::msr::msr_status::ok;
        if (r.have_procbased2) r.procbased2 = vmx::caps::parse_procbased_ctls2(procbased2.value);

        const auto exit_ctls = vmx::msr::read_msr(ia32_vmx_exit_ctls);
        r.have_exit = exit_ctls.status == vmx::msr::msr_status::ok;
        if (r.have_exit) r.exit_ctls = vmx::caps::parse_exit_ctls(exit_ctls.value);

        const auto entry_ctls = vmx::msr::read_msr(ia32_vmx_entry_ctls);
        r.have_entry = entry_ctls.status == vmx::msr::msr_status::ok;
        if (r.have_entry) r.entry_ctls = vmx::caps::parse_entry_ctls(entry_ctls.value);

        const auto ept_vpid = vmx::msr::read_msr(ia32_vmx_ept_vpid_cap);
        r.have_ept_vpid = ept_vpid.status == vmx::msr::msr_status::ok;
        if (r.have_ept_vpid) r.ept_vpid = vmx::caps::parse_ept_vpid_cap(ept_vpid.value);

        return r;
    }

    void render_text(const report& r)
    {
        vmx::print_kv("vendor", printable(r.vendor));
        vmx::print_kv("family/model/stepping",
                      std::format("{:02x}_{:02x}_{}", r.sig.family, r.sig.model, r.sig.stepping));
        vmx::print_kv("vmx (intel vt-x)", r.vmx_supported ? "supported" : "not supported",
                      r.vmx_supported ? vmx::kv_status::ok : vmx::kv_status::bad);
        if (r.is_amd)
        {
            vmx::print_kv("svm (amd-v)", r.svm_supported ? "supported" : "not supported",
                          r.svm_supported ? vmx::kv_status::ok : vmx::kv_status::bad);
        }
        else
        {
            vmx::print_kv("svm (amd-v)", "n/a");
        }
        if (r.hypervisor_present)
        {
            vmx::print_kv("hypervisor present", std::format("yes ({})", printable(r.hypervisor_id)));
        }
        else
        {
            vmx::print_kv("hypervisor present", "no");
        }

        if (!r.have_msr)
        {
            vmx::print_kv("msr", msr_status_hint(r.msr_status), vmx::kv_status::bad);
            return;
        }

        vmx::caps::print(r.basic);
        if (r.have_pinbased) vmx::caps::print(r.pinbased);
        if (r.have_procbased) vmx::caps::print(r.procbased);
        if (r.have_procbased2) vmx::caps::print(r.procbased2);
        if (r.have_exit) vmx::caps::print(r.exit_ctls);
        if (r.have_entry) vmx::caps::print(r.entry_ctls);
        if (r.have_ept_vpid) vmx::caps::print(r.ept_vpid);
    }

    void render_json(const report& r)
    {
        vmx::json::json_writer w(std::cout);
        w.begin_object();
        w.key_string("vendor", r.vendor);
        w.key_uint("family", r.sig.family);
        w.key_uint("model", r.sig.model);
        w.key_uint("stepping", r.sig.stepping);
        w.key_bool("vmx_supported", r.vmx_supported);
        if (r.is_amd)
        {
            w.key_bool("svm_supported", r.svm_supported);
        }
        w.key_bool("hypervisor_present", r.hypervisor_present);
        if (r.hypervisor_present) w.key_string("hypervisor_vendor", r.hypervisor_id);

        w.key_bool("msr_available", r.have_msr);
        if (!r.have_msr)
        {
            w.key_string("msr_status", msr_status_hint(r.msr_status));
            w.end_object();
            return;
        }

        vmx::caps::to_json(w, r.basic);
        if (r.have_pinbased) vmx::caps::to_json(w, r.pinbased);
        if (r.have_procbased) vmx::caps::to_json(w, r.procbased);
        if (r.have_procbased2) vmx::caps::to_json(w, r.procbased2);
        if (r.have_exit) vmx::caps::to_json(w, r.exit_ctls);
        if (r.have_entry) vmx::caps::to_json(w, r.entry_ctls);
        if (r.have_ept_vpid) vmx::caps::to_json(w, r.ept_vpid);
        w.end_object();
    }
}

int main(int argc, char** argv)
{
    const auto opts = parse_args(std::span(argv, static_cast<size_t>(argc)));
    if (!opts.unknown.empty())
    {
        std::cerr << "unknown flag: " << opts.unknown << '\n';
        print_usage(std::cerr);
        return 2;
    }
    if (opts.help)
    {
        print_help(std::cout);
        return 0;
    }
    if (opts.no_color) vmx::disable_color();

    const auto r = gather();
    if (opts.mode == output_mode::json)
    {
        render_json(r);
    }
    else
    {
        render_text(r);
    }
    return 0;
}
