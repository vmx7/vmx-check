#pragma once

#include <cstdint>

#include "../printer/json.hpp"

namespace vmx::caps
{
    struct ia32_vmx_basic_fields
    {
        uint32_t revision_id;
        uint32_t vmcs_region_size;
        bool physical_width_32bit;
        bool dual_monitor_smm;
        uint8_t memory_type;
        bool ins_outs_reporting;
        bool true_controls;
    };

    [[nodiscard]] ia32_vmx_basic_fields parse_basic(uint64_t raw) noexcept;

    void print(const ia32_vmx_basic_fields & f);

    void to_json(json::json_writer & w, const ia32_vmx_basic_fields & f);
}
