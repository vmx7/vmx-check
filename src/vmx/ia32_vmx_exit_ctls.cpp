#include "ia32_vmx_exit_ctls.hpp"

#include "../printer.hpp"

namespace vmx::caps
{
    // sdm vol.3 a.4, ia32_vmx_exit_ctls (msr 0x483); allowed-1 in bits 63:32
    ia32_vmx_exit_ctls_fields parse_exit_ctls(uint64_t raw) noexcept
    {
        const uint32_t allowed_1 = static_cast<uint32_t>(raw >> 32);
        const auto allows = [allowed_1](uint32_t bit)
        {
            return (allowed_1 & (1u << bit)) != 0;
        };

        ia32_vmx_exit_ctls_fields f{};
        f.save_debug_controls = allows(2);
        f.host_address_space_size = allows(9);
        f.load_ia32_perf_global_ctrl = allows(12);
        f.acknowledge_interrupt_on_exit = allows(15);
        f.save_ia32_pat = allows(18);
        f.load_ia32_pat = allows(19);
        f.save_ia32_efer = allows(20);
        f.load_ia32_efer = allows(21);
        f.save_vmx_preemption_timer = allows(22);
        f.clear_ia32_bndcfgs = allows(23);
        f.conceal_vmx_from_pt = allows(24);
        return f;
    }

    void print(const ia32_vmx_exit_ctls_fields& f)
    {
        print_section("ia32_vmx_exit_ctls");
        print_kv("    save_debug_controls", f.save_debug_controls ? "yes" : "no");
        print_kv("    host_address_space_size", f.host_address_space_size ? "yes" : "no");
        print_kv("    load_ia32_perf_global_ctrl", f.load_ia32_perf_global_ctrl ? "yes" : "no");
        print_kv("    acknowledge_interrupt_on_exit",
                 f.acknowledge_interrupt_on_exit ? "yes" : "no");
        print_kv("    save_ia32_pat", f.save_ia32_pat ? "yes" : "no");
        print_kv("    load_ia32_pat", f.load_ia32_pat ? "yes" : "no");
        print_kv("    save_ia32_efer", f.save_ia32_efer ? "yes" : "no");
        print_kv("    load_ia32_efer", f.load_ia32_efer ? "yes" : "no");
        print_kv("    save_vmx_preemption_timer", f.save_vmx_preemption_timer ? "yes" : "no");
        print_kv("    clear_ia32_bndcfgs", f.clear_ia32_bndcfgs ? "yes" : "no");
        print_kv("    conceal_vmx_from_pt", f.conceal_vmx_from_pt ? "yes" : "no");
    }

    void to_json(json::json_writer & w, const ia32_vmx_exit_ctls_fields& f)
    {
        w.begin_object("ia32_vmx_exit_ctls");
        w.key_bool("save_debug_controls", f.save_debug_controls);
        w.key_bool("host_address_space_size", f.host_address_space_size);
        w.key_bool("load_ia32_perf_global_ctrl", f.load_ia32_perf_global_ctrl);
        w.key_bool("acknowledge_interrupt_on_exit", f.acknowledge_interrupt_on_exit);
        w.key_bool("save_ia32_pat", f.save_ia32_pat);
        w.key_bool("load_ia32_pat", f.load_ia32_pat);
        w.key_bool("save_ia32_efer", f.save_ia32_efer);
        w.key_bool("load_ia32_efer", f.load_ia32_efer);
        w.key_bool("save_vmx_preemption_timer", f.save_vmx_preemption_timer);
        w.key_bool("clear_ia32_bndcfgs", f.clear_ia32_bndcfgs);
        w.key_bool("conceal_vmx_from_pt", f.conceal_vmx_from_pt);
        w.end_object();
    }
}
