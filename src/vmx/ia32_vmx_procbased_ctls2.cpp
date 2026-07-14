#include "ia32_vmx_procbased_ctls2.hpp"

#include "../printer.hpp"

namespace vmx::caps
{
    // sdm vol.3 a.3.3, ia32_vmx_procbased_ctls2 (msr 0x48b); allowed-1 in bits 63:32
    ia32_vmx_procbased_ctls2_fields parse_procbased_ctls2(uint64_t raw) noexcept
    {
        const uint32_t allowed_1 = static_cast<uint32_t>(raw >> 32);
        const auto allows = [allowed_1](uint32_t bit)
        {
            return (allowed_1 & (1u << bit)) != 0;
        };

        ia32_vmx_procbased_ctls2_fields f{};
        f.virtualize_apic_accesses = allows(0);
        f.enable_ept = allows(1);
        f.descriptor_table_exiting = allows(2);
        f.enable_rdtscp = allows(3);
        f.virtualize_x2apic_mode = allows(4);
        f.enable_vpid = allows(5);
        f.wbinvd_exiting = allows(6);
        f.unrestricted_guest = allows(7);
        f.apic_register_virtualization = allows(8);
        f.virtual_interrupt_delivery = allows(9);
        f.pause_loop_exiting = allows(10);
        f.rdrand_exiting = allows(11);
        f.enable_invpcid = allows(12);
        f.enable_vm_functions = allows(13);
        f.vmcs_shadowing = allows(14);
        f.enable_encls_exiting = allows(15);
        f.rdseed_exiting = allows(16);
        f.enable_pml = allows(17);
        f.ept_violation_ve = allows(18);
        f.enable_xsaves_xrstors = allows(20);
        f.mode_based_execute_control_ept = allows(22);
        f.use_tsc_scaling = allows(25);
        return f;
    }

    void print(const ia32_vmx_procbased_ctls2_fields& f)
    {
        print_section("ia32_vmx_procbased_ctls2");
        print_kv("    virtualize_apic_accesses", f.virtualize_apic_accesses ? "yes" : "no");
        print_kv("    enable_ept", f.enable_ept ? "yes" : "no");
        print_kv("    descriptor_table_exiting", f.descriptor_table_exiting ? "yes" : "no");
        print_kv("    enable_rdtscp", f.enable_rdtscp ? "yes" : "no");
        print_kv("    virtualize_x2apic_mode", f.virtualize_x2apic_mode ? "yes" : "no");
        print_kv("    enable_vpid", f.enable_vpid ? "yes" : "no");
        print_kv("    wbinvd_exiting", f.wbinvd_exiting ? "yes" : "no");
        print_kv("    unrestricted_guest", f.unrestricted_guest ? "yes" : "no");
        print_kv("    apic_register_virtualization",
                 f.apic_register_virtualization ? "yes" : "no");
        print_kv("    virtual_interrupt_delivery", f.virtual_interrupt_delivery ? "yes" : "no");
        print_kv("    pause_loop_exiting", f.pause_loop_exiting ? "yes" : "no");
        print_kv("    rdrand_exiting", f.rdrand_exiting ? "yes" : "no");
        print_kv("    enable_invpcid", f.enable_invpcid ? "yes" : "no");
        print_kv("    enable_vm_functions", f.enable_vm_functions ? "yes" : "no");
        print_kv("    vmcs_shadowing", f.vmcs_shadowing ? "yes" : "no");
        print_kv("    enable_encls_exiting", f.enable_encls_exiting ? "yes" : "no");
        print_kv("    rdseed_exiting", f.rdseed_exiting ? "yes" : "no");
        print_kv("    enable_pml", f.enable_pml ? "yes" : "no");
        print_kv("    ept_violation_ve", f.ept_violation_ve ? "yes" : "no");
        print_kv("    enable_xsaves_xrstors", f.enable_xsaves_xrstors ? "yes" : "no");
        print_kv("    mode_based_execute_control_ept",
                 f.mode_based_execute_control_ept ? "yes" : "no");
        print_kv("    use_tsc_scaling", f.use_tsc_scaling ? "yes" : "no");
    }
}
