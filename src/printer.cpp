#include "printer.hpp"

#include <cstdio>
#include <iostream>

#if defined(_WIN32)
#include <io.h>
#else
#include <unistd.h>
#endif

namespace vmx
{
    namespace
    {
        constexpr std::string_view color_green = "\x1b[32m";
        constexpr std::string_view color_red = "\x1b[31m";
        constexpr std::string_view color_bold = "\x1b[1m";
        constexpr std::string_view color_reset = "\x1b[0m";

        bool color_disabled = false;

        bool stdout_is_tty()
        {
#if defined(_WIN32)
            return _isatty(_fileno(stdout)) != 0;
#else
            return isatty(fileno(stdout)) != 0;
#endif
        }

        bool use_color()
        {
            static const bool tty = stdout_is_tty();
            return tty && !color_disabled;
        }

        std::string_view status_color(kv_status s)
        {
            switch (s)
            {
            case kv_status::ok:
                return color_green;
            case kv_status::bad:
                return color_red;
            case kv_status::info:
                return {};
            }
            return {};
        }
    }

    void disable_color()
    {
        color_disabled = true;
    }

    void print_section(std::string_view name)
    {
        const std::string_view color = use_color() ? color_bold : std::string_view{};
        const std::string_view reset = color.empty() ? std::string_view{} : color_reset;
        std::cout << '\n' << color << '[' << name << ']' << reset << '\n';
    }

    void print_kv(std::string_view key, std::string_view value, kv_status s)
    {
        const std::string_view color = use_color() ? status_color(s) : std::string_view{};
        const std::string_view reset = color.empty() ? std::string_view{} : color_reset;
        std::cout << key << ": " << color << value << reset << '\n';
    }
}
