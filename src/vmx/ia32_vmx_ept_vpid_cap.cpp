#include "ia32_vmx_ept_vpid_cap.hpp"

#include "../printer.hpp"

namespace vmx::caps
{
    // sdm vol.3 a.10, ia32_vmx_ept_vpid_cap (msr 0x48c); direct capability bitmap
    ia32_vmx_ept_vpid_cap_fields parse_ept_vpid_cap(uint64_t raw) noexcept
    {
        const auto has = [raw](uint32_t bit)
        {
            return (raw & (uint64_t{1} << bit)) != 0;
        };

        ia32_vmx_ept_vpid_cap_fields f{};
        f.execute_only_translation = has(0);
        f.page_walk_length_4 = has(6);
        f.memory_type_uncacheable = has(8);
        f.memory_type_writeback = has(14);
        f.pde_2mb_pages = has(16);
        f.pdpte_1gb_pages = has(17);
        f.invept_supported = has(20);
        f.ept_accessed_dirty_flags = has(21);
        f.advanced_ept_violations = has(22);
        f.invept_single_context = has(25);
        f.invept_all_context = has(26);
        f.invvpid_supported = has(32);
        f.invvpid_individual_address = has(40);
        f.invvpid_single_context = has(41);
        f.invvpid_all_context = has(42);
        f.invvpid_single_context_retaining_globals = has(43);
        return f;
    }

    void print(const ia32_vmx_ept_vpid_cap_fields & f)
    {
        print_section("ia32_vmx_ept_vpid_cap");
        print_kv("    execute_only_translation", f.execute_only_translation ? "yes" : "no");
        print_kv("    page_walk_length_4", f.page_walk_length_4 ? "yes" : "no");
        print_kv("    memory_type_uncacheable", f.memory_type_uncacheable ? "yes" : "no");
        print_kv("    memory_type_writeback", f.memory_type_writeback ? "yes" : "no");
        print_kv("    pde_2mb_pages", f.pde_2mb_pages ? "yes" : "no");
        print_kv("    pdpte_1gb_pages", f.pdpte_1gb_pages ? "yes" : "no");
        print_kv("    invept_supported", f.invept_supported ? "yes" : "no");
        print_kv("    ept_accessed_dirty_flags", f.ept_accessed_dirty_flags ? "yes" : "no");
        print_kv("    advanced_ept_violations", f.advanced_ept_violations ? "yes" : "no");
        print_kv("    invept_single_context", f.invept_single_context ? "yes" : "no");
        print_kv("    invept_all_context", f.invept_all_context ? "yes" : "no");
        print_kv("    invvpid_supported", f.invvpid_supported ? "yes" : "no");
        print_kv("    invvpid_individual_address", f.invvpid_individual_address ? "yes" : "no");
        print_kv("    invvpid_single_context", f.invvpid_single_context ? "yes" : "no");
        print_kv("    invvpid_all_context", f.invvpid_all_context ? "yes" : "no");
        print_kv("    invvpid_single_context_retaining_globals",
                 f.invvpid_single_context_retaining_globals ? "yes" : "no");
    }

    void to_json(json::json_writer & w, const ia32_vmx_ept_vpid_cap_fields & f)
    {
        w.begin_object("ia32_vmx_ept_vpid_cap");
        w.key_bool("execute_only_translation", f.execute_only_translation);
        w.key_bool("page_walk_length_4", f.page_walk_length_4);
        w.key_bool("memory_type_uncacheable", f.memory_type_uncacheable);
        w.key_bool("memory_type_writeback", f.memory_type_writeback);
        w.key_bool("pde_2mb_pages", f.pde_2mb_pages);
        w.key_bool("pdpte_1gb_pages", f.pdpte_1gb_pages);
        w.key_bool("invept_supported", f.invept_supported);
        w.key_bool("ept_accessed_dirty_flags", f.ept_accessed_dirty_flags);
        w.key_bool("advanced_ept_violations", f.advanced_ept_violations);
        w.key_bool("invept_single_context", f.invept_single_context);
        w.key_bool("invept_all_context", f.invept_all_context);
        w.key_bool("invvpid_supported", f.invvpid_supported);
        w.key_bool("invvpid_individual_address", f.invvpid_individual_address);
        w.key_bool("invvpid_single_context", f.invvpid_single_context);
        w.key_bool("invvpid_all_context", f.invvpid_all_context);
        w.key_bool("invvpid_single_context_retaining_globals",
                   f.invvpid_single_context_retaining_globals);
        w.end_object();
    }
}
