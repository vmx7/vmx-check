#include "ia32_vmx_entry_ctls.hpp"

#include "../printer.hpp"

namespace vmx::caps
{
    // sdm vol.3 a.5, ia32_vmx_entry_ctls (msr 0x484); allowed-1 in bits 63:32
    ia32_vmx_entry_ctls_fields parse_entry_ctls(uint64_t raw) noexcept
    {
        const uint32_t allowed_1 = static_cast<uint32_t>(raw >> 32);
        const auto allows = [allowed_1](uint32_t bit)
        {
            return (allowed_1 & (1u << bit)) != 0;
        };

        ia32_vmx_entry_ctls_fields f{};
        f.load_debug_controls = allows(2);
        f.ia32e_mode_guest = allows(9);
        f.entry_to_smm = allows(10);
        f.deactivate_dual_monitor = allows(11);
        f.load_ia32_perf_global_ctrl = allows(13);
        f.load_ia32_pat = allows(14);
        f.load_ia32_efer = allows(15);
        f.load_ia32_bndcfgs = allows(16);
        f.conceal_vmx_from_pt = allows(17);
        return f;
    }

    void print(const ia32_vmx_entry_ctls_fields& f)
    {
        print_section("ia32_vmx_entry_ctls");
        print_kv("    load_debug_controls", f.load_debug_controls ? "yes" : "no");
        print_kv("    ia32e_mode_guest", f.ia32e_mode_guest ? "yes" : "no");
        print_kv("    entry_to_smm", f.entry_to_smm ? "yes" : "no");
        print_kv("    deactivate_dual_monitor", f.deactivate_dual_monitor ? "yes" : "no");
        print_kv("    load_ia32_perf_global_ctrl", f.load_ia32_perf_global_ctrl ? "yes" : "no");
        print_kv("    load_ia32_pat", f.load_ia32_pat ? "yes" : "no");
        print_kv("    load_ia32_efer", f.load_ia32_efer ? "yes" : "no");
        print_kv("    load_ia32_bndcfgs", f.load_ia32_bndcfgs ? "yes" : "no");
        print_kv("    conceal_vmx_from_pt", f.conceal_vmx_from_pt ? "yes" : "no");
    }

    void to_json(json::json_writer & w, const ia32_vmx_entry_ctls_fields& f)
    {
        w.begin_object("ia32_vmx_entry_ctls");
        w.key_bool("load_debug_controls", f.load_debug_controls);
        w.key_bool("ia32e_mode_guest", f.ia32e_mode_guest);
        w.key_bool("entry_to_smm", f.entry_to_smm);
        w.key_bool("deactivate_dual_monitor", f.deactivate_dual_monitor);
        w.key_bool("load_ia32_perf_global_ctrl", f.load_ia32_perf_global_ctrl);
        w.key_bool("load_ia32_pat", f.load_ia32_pat);
        w.key_bool("load_ia32_efer", f.load_ia32_efer);
        w.key_bool("load_ia32_bndcfgs", f.load_ia32_bndcfgs);
        w.key_bool("conceal_vmx_from_pt", f.conceal_vmx_from_pt);
        w.end_object();
    }
}
