#pragma once

#include <cstdint>

namespace vmx::caps
{
    struct ia32_vmx_pinbased_ctls_fields
    {
        bool external_interrupt_exiting;
        bool nmi_exiting;
        bool virtual_nmis;
        bool activate_vmx_preemption_timer;
        bool process_posted_interrupts;
    };

    [[nodiscard]] ia32_vmx_pinbased_ctls_fields parse_pinbased_ctls(uint64_t raw) noexcept;

    void print(const ia32_vmx_pinbased_ctls_fields& f);
}
