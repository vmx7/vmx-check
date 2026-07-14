#pragma once

#include <string_view>

namespace vmx
{
    enum class kv_status
    {
        ok,
        bad,
        info
    };

    void disable_color();

    void print_section(std::string_view name);

    void print_kv(std::string_view key, std::string_view value, kv_status s = kv_status::info);
}
