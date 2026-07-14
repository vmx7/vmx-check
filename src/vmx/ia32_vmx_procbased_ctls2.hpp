#pragma once

#include <cstdint>

#include "../printer/json.hpp"

namespace vmx::caps
{
    struct ia32_vmx_procbased_ctls2_fields
    {
        bool virtualize_apic_accesses;
        bool enable_ept;
        bool descriptor_table_exiting;
        bool enable_rdtscp;
        bool virtualize_x2apic_mode;
        bool enable_vpid;
        bool wbinvd_exiting;
        bool unrestricted_guest;
        bool apic_register_virtualization;
        bool virtual_interrupt_delivery;
        bool pause_loop_exiting;
        bool rdrand_exiting;
        bool enable_invpcid;
        bool enable_vm_functions;
        bool vmcs_shadowing;
        bool enable_encls_exiting;
        bool rdseed_exiting;
        bool enable_pml;
        bool ept_violation_ve;
        bool enable_xsaves_xrstors;
        bool mode_based_execute_control_ept;
        bool use_tsc_scaling;
    };

    [[nodiscard]] ia32_vmx_procbased_ctls2_fields parse_procbased_ctls2(uint64_t raw) noexcept;

    void print(const ia32_vmx_procbased_ctls2_fields & f);

    void to_json(json::json_writer & w, const ia32_vmx_procbased_ctls2_fields & f);
}
