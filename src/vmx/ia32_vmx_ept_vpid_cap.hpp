#pragma once

#include <cstdint>

#include "../printer/json.hpp"

namespace vmx::caps
{
    struct ia32_vmx_ept_vpid_cap_fields
    {
        bool execute_only_translation;
        bool page_walk_length_4;
        bool memory_type_uncacheable;
        bool memory_type_writeback;
        bool pde_2mb_pages;
        bool pdpte_1gb_pages;
        bool invept_supported;
        bool ept_accessed_dirty_flags;
        bool advanced_ept_violations;
        bool invept_single_context;
        bool invept_all_context;
        bool invvpid_supported;
        bool invvpid_individual_address;
        bool invvpid_single_context;
        bool invvpid_all_context;
        bool invvpid_single_context_retaining_globals;
    };

    [[nodiscard]] ia32_vmx_ept_vpid_cap_fields parse_ept_vpid_cap(uint64_t raw) noexcept;

    void print(const ia32_vmx_ept_vpid_cap_fields & f);

    void to_json(json::json_writer & w, const ia32_vmx_ept_vpid_cap_fields & f);
}
