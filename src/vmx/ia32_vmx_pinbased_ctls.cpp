#include "ia32_vmx_pinbased_ctls.hpp"

#include "../printer.hpp"

namespace vmx::caps
{
    // sdm vol.3 a.3.1, ia32_vmx_pinbased_ctls (msr 0x481); allowed-1 in bits 63:32
    ia32_vmx_pinbased_ctls_fields parse_pinbased_ctls(uint64_t raw) noexcept
    {
        const uint32_t allowed_1 = static_cast<uint32_t>(raw >> 32);
        const auto allows = [allowed_1](uint32_t bit)
        {
            return (allowed_1 & (1u << bit)) != 0;
        };

        ia32_vmx_pinbased_ctls_fields f{};
        f.external_interrupt_exiting = allows(0);
        f.nmi_exiting = allows(3);
        f.virtual_nmis = allows(5);
        f.activate_vmx_preemption_timer = allows(6);
        f.process_posted_interrupts = allows(7);
        return f;
    }

    void print(const ia32_vmx_pinbased_ctls_fields& f)
    {
        print_section("ia32_vmx_pinbased_ctls");
        print_kv("    external_interrupt_exiting", f.external_interrupt_exiting ? "yes" : "no");
        print_kv("    nmi_exiting", f.nmi_exiting ? "yes" : "no");
        print_kv("    virtual_nmis", f.virtual_nmis ? "yes" : "no");
        print_kv("    activate_vmx_preemption_timer",
                 f.activate_vmx_preemption_timer ? "yes" : "no");
        print_kv("    process_posted_interrupts", f.process_posted_interrupts ? "yes" : "no");
    }

    void to_json(json::json_writer & w, const ia32_vmx_pinbased_ctls_fields& f)
    {
        w.begin_object("ia32_vmx_pinbased_ctls");
        w.key_bool("external_interrupt_exiting", f.external_interrupt_exiting);
        w.key_bool("nmi_exiting", f.nmi_exiting);
        w.key_bool("virtual_nmis", f.virtual_nmis);
        w.key_bool("activate_vmx_preemption_timer", f.activate_vmx_preemption_timer);
        w.key_bool("process_posted_interrupts", f.process_posted_interrupts);
        w.end_object();
    }
}
