#pragma once

#include <cstdint>
#include <ostream>
#include <string_view>
#include <vector>

namespace vmx::json
{
    class json_writer
    {
    public:
        explicit json_writer(std::ostream & out) noexcept;

        void begin_object();
        void begin_object(std::string_view key);
        void end_object();

        void begin_array(std::string_view key);
        void end_array();

        void key_string(std::string_view key, std::string_view value);
        void key_bool(std::string_view key, bool value);
        void key_uint(std::string_view key, uint64_t value);
        void key_hex(std::string_view key, uint64_t value);

    private:
        void write_indent();
        void write_separator();
        void write_key(std::string_view key);
        void write_escaped(std::string_view value);

        std::ostream & out_;
        std::vector<bool> needs_comma_;
    };
}
