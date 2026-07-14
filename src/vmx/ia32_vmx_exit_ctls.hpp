#pragma once

#include <cstdint>

namespace vmx::caps
{
    struct ia32_vmx_exit_ctls_fields
    {
        bool save_debug_controls;
        bool host_address_space_size;
        bool load_ia32_perf_global_ctrl;
        bool acknowledge_interrupt_on_exit;
        bool save_ia32_pat;
        bool load_ia32_pat;
        bool save_ia32_efer;
        bool load_ia32_efer;
        bool save_vmx_preemption_timer;
        bool clear_ia32_bndcfgs;
        bool conceal_vmx_from_pt;
    };

    [[nodiscard]] ia32_vmx_exit_ctls_fields parse_exit_ctls(uint64_t raw) noexcept;

    void print(const ia32_vmx_exit_ctls_fields& f);
}
