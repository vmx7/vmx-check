#pragma once

#include <cstdint>

namespace vmx::caps
{
    struct ia32_vmx_procbased_ctls_fields
    {
        bool interrupt_window_exiting;
        bool use_tsc_offsetting;
        bool hlt_exiting;
        bool invlpg_exiting;
        bool mwait_exiting;
        bool rdpmc_exiting;
        bool rdtsc_exiting;
        bool cr3_load_exiting;
        bool cr3_store_exiting;
        bool cr8_load_exiting;
        bool cr8_store_exiting;
        bool use_tpr_shadow;
        bool nmi_window_exiting;
        bool mov_dr_exiting;
        bool unconditional_io_exiting;
        bool use_io_bitmaps;
        bool monitor_trap_flag;
        bool use_msr_bitmaps;
        bool monitor_exiting;
        bool pause_exiting;
        bool activate_secondary_controls;
    };

    [[nodiscard]] ia32_vmx_procbased_ctls_fields parse_procbased_ctls(uint64_t raw) noexcept;

    void print(const ia32_vmx_procbased_ctls_fields& f);
}
