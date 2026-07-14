#pragma once

#include <cstdint>

#include "../printer/json.hpp"

namespace vmx::caps
{
    struct ia32_vmx_entry_ctls_fields
    {
        bool load_debug_controls;
        bool ia32e_mode_guest;
        bool entry_to_smm;
        bool deactivate_dual_monitor;
        bool load_ia32_perf_global_ctrl;
        bool load_ia32_pat;
        bool load_ia32_efer;
        bool load_ia32_bndcfgs;
        bool conceal_vmx_from_pt;
    };

    [[nodiscard]] ia32_vmx_entry_ctls_fields parse_entry_ctls(uint64_t raw) noexcept;

    void print(const ia32_vmx_entry_ctls_fields & f);

    void to_json(json::json_writer & w, const ia32_vmx_entry_ctls_fields & f);
}
