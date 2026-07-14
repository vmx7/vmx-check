#include "ia32_vmx_procbased_ctls.hpp"

#include "../printer.hpp"

namespace vmx::caps
{
    // sdm vol.3 a.3.2, ia32_vmx_procbased_ctls (msr 0x482); allowed-1 in bits 63:32
    ia32_vmx_procbased_ctls_fields parse_procbased_ctls(uint64_t raw) noexcept
    {
        const uint32_t allowed_1 = static_cast<uint32_t>(raw >> 32);
        const auto allows = [allowed_1](uint32_t bit)
        {
            return (allowed_1 & (1u << bit)) != 0;
        };

        ia32_vmx_procbased_ctls_fields f{};
        f.interrupt_window_exiting = allows(2);
        f.use_tsc_offsetting = allows(3);
        f.hlt_exiting = allows(7);
        f.invlpg_exiting = allows(9);
        f.mwait_exiting = allows(10);
        f.rdpmc_exiting = allows(11);
        f.rdtsc_exiting = allows(12);
        f.cr3_load_exiting = allows(15);
        f.cr3_store_exiting = allows(16);
        f.cr8_load_exiting = allows(19);
        f.cr8_store_exiting = allows(20);
        f.use_tpr_shadow = allows(21);
        f.nmi_window_exiting = allows(22);
        f.mov_dr_exiting = allows(23);
        f.unconditional_io_exiting = allows(24);
        f.use_io_bitmaps = allows(25);
        f.monitor_trap_flag = allows(27);
        f.use_msr_bitmaps = allows(28);
        f.monitor_exiting = allows(29);
        f.pause_exiting = allows(30);
        f.activate_secondary_controls = allows(31);
        return f;
    }

    void print(const ia32_vmx_procbased_ctls_fields& f)
    {
        print_section("ia32_vmx_procbased_ctls");
        print_kv("    interrupt_window_exiting", f.interrupt_window_exiting ? "yes" : "no");
        print_kv("    use_tsc_offsetting", f.use_tsc_offsetting ? "yes" : "no");
        print_kv("    hlt_exiting", f.hlt_exiting ? "yes" : "no");
        print_kv("    invlpg_exiting", f.invlpg_exiting ? "yes" : "no");
        print_kv("    mwait_exiting", f.mwait_exiting ? "yes" : "no");
        print_kv("    rdpmc_exiting", f.rdpmc_exiting ? "yes" : "no");
        print_kv("    rdtsc_exiting", f.rdtsc_exiting ? "yes" : "no");
        print_kv("    cr3_load_exiting", f.cr3_load_exiting ? "yes" : "no");
        print_kv("    cr3_store_exiting", f.cr3_store_exiting ? "yes" : "no");
        print_kv("    cr8_load_exiting", f.cr8_load_exiting ? "yes" : "no");
        print_kv("    cr8_store_exiting", f.cr8_store_exiting ? "yes" : "no");
        print_kv("    use_tpr_shadow", f.use_tpr_shadow ? "yes" : "no");
        print_kv("    nmi_window_exiting", f.nmi_window_exiting ? "yes" : "no");
        print_kv("    mov_dr_exiting", f.mov_dr_exiting ? "yes" : "no");
        print_kv("    unconditional_io_exiting", f.unconditional_io_exiting ? "yes" : "no");
        print_kv("    use_io_bitmaps", f.use_io_bitmaps ? "yes" : "no");
        print_kv("    monitor_trap_flag", f.monitor_trap_flag ? "yes" : "no");
        print_kv("    use_msr_bitmaps", f.use_msr_bitmaps ? "yes" : "no");
        print_kv("    monitor_exiting", f.monitor_exiting ? "yes" : "no");
        print_kv("    pause_exiting", f.pause_exiting ? "yes" : "no");
        print_kv("    activate_secondary_controls", f.activate_secondary_controls ? "yes" : "no");
    }

    void to_json(json::json_writer & w, const ia32_vmx_procbased_ctls_fields& f)
    {
        w.begin_object("ia32_vmx_procbased_ctls");
        w.key_bool("interrupt_window_exiting", f.interrupt_window_exiting);
        w.key_bool("use_tsc_offsetting", f.use_tsc_offsetting);
        w.key_bool("hlt_exiting", f.hlt_exiting);
        w.key_bool("invlpg_exiting", f.invlpg_exiting);
        w.key_bool("mwait_exiting", f.mwait_exiting);
        w.key_bool("rdpmc_exiting", f.rdpmc_exiting);
        w.key_bool("rdtsc_exiting", f.rdtsc_exiting);
        w.key_bool("cr3_load_exiting", f.cr3_load_exiting);
        w.key_bool("cr3_store_exiting", f.cr3_store_exiting);
        w.key_bool("cr8_load_exiting", f.cr8_load_exiting);
        w.key_bool("cr8_store_exiting", f.cr8_store_exiting);
        w.key_bool("use_tpr_shadow", f.use_tpr_shadow);
        w.key_bool("nmi_window_exiting", f.nmi_window_exiting);
        w.key_bool("mov_dr_exiting", f.mov_dr_exiting);
        w.key_bool("unconditional_io_exiting", f.unconditional_io_exiting);
        w.key_bool("use_io_bitmaps", f.use_io_bitmaps);
        w.key_bool("monitor_trap_flag", f.monitor_trap_flag);
        w.key_bool("use_msr_bitmaps", f.use_msr_bitmaps);
        w.key_bool("monitor_exiting", f.monitor_exiting);
        w.key_bool("pause_exiting", f.pause_exiting);
        w.key_bool("activate_secondary_controls", f.activate_secondary_controls);
        w.end_object();
    }
}
