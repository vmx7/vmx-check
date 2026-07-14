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
        constexpr std::string_view color_reset = "\x1b[0m";

        bool stdout_is_tty()
        {
#if defined(_WIN32)
            return _isatty(_fileno(stdout)) != 0;
#else
            return isatty(fileno(stdout)) != 0;
#endif
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

    void print_kv(std::string_view key, std::string_view value, kv_status s)
    {
        static const bool colored = stdout_is_tty();
        const std::string_view color = colored ? status_color(s) : std::string_view{};
        const std::string_view reset = color.empty() ? std::string_view{} : color_reset;
        std::cout << key << ": " << color << value << reset << '\n';
    }
}
