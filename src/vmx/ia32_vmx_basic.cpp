#include "ia32_vmx_basic.hpp"

#include <format>

#include "../printer.hpp"

namespace vmx::caps
{
    // sdm vol.3 a.1, ia32_vmx_basic (msr 0x480)
    ia32_vmx_basic_fields parse_basic(uint64_t raw) noexcept
    {
        ia32_vmx_basic_fields f{};
        f.revision_id = static_cast<uint32_t>(raw & 0x7fffffff);
        f.vmcs_region_size = static_cast<uint32_t>((raw >> 32) & 0x1fff);
        f.physical_width_32bit = ((raw >> 48) & 0x1) != 0;
        f.dual_monitor_smm = ((raw >> 49) & 0x1) != 0;
        f.memory_type = static_cast<uint8_t>((raw >> 50) & 0xf);
        f.ins_outs_reporting = ((raw >> 54) & 0x1) != 0;
        f.true_controls = ((raw >> 55) & 0x1) != 0;
        return f;
    }

    void print(const ia32_vmx_basic_fields & f)
    {
        print_section("ia32_vmx_basic");
        print_kv("    revision_id", std::format("{:#x}", f.revision_id));
        print_kv("    vmcs_region_size", std::format("{}", f.vmcs_region_size));
        print_kv("    physical_width_32bit", f.physical_width_32bit ? "yes" : "no");
        print_kv("    dual_monitor_smm", f.dual_monitor_smm ? "yes" : "no");
        print_kv("    memory_type", std::format("{}", f.memory_type));
        print_kv("    ins_outs_reporting", f.ins_outs_reporting ? "yes" : "no");
        print_kv("    true_controls", f.true_controls ? "yes" : "no");
    }

    void to_json(json::json_writer & w, const ia32_vmx_basic_fields & f)
    {
        w.begin_object("ia32_vmx_basic");
        w.key_hex("revision_id", f.revision_id);
        w.key_uint("vmcs_region_size", f.vmcs_region_size);
        w.key_bool("physical_width_32bit", f.physical_width_32bit);
        w.key_bool("dual_monitor_smm", f.dual_monitor_smm);
        w.key_uint("memory_type", f.memory_type);
        w.key_bool("ins_outs_reporting", f.ins_outs_reporting);
        w.key_bool("true_controls", f.true_controls);
        w.end_object();
    }
}
