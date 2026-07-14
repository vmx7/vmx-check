#include "json.hpp"

#include <format>

namespace vmx::json
{
    json_writer::json_writer(std::ostream & out) noexcept : out_(out)
    {
    }

    void json_writer::write_indent()
    {
        for (size_t level = 0; level < needs_comma_.size(); ++level)
        {
            out_ << "    ";
        }
    }

    void json_writer::write_separator()
    {
        if (!needs_comma_.empty())
        {
            if (needs_comma_.back()) out_ << ',';
            needs_comma_.back() = true;
            out_ << '\n';
        }
        write_indent();
    }

    void json_writer::write_key(std::string_view key)
    {
        write_escaped(key);
        out_ << ": ";
    }

    void json_writer::write_escaped(std::string_view value)
    {
        out_ << '"';
        for (const char c : value)
        {
            switch (c)
            {
            case '"':
                out_ << "\\\"";
                break;
            case '\\':
                out_ << "\\\\";
                break;
            case '\n':
                out_ << "\\n";
                break;
            case '\r':
                out_ << "\\r";
                break;
            case '\t':
                out_ << "\\t";
                break;
            case '\b':
                out_ << "\\b";
                break;
            case '\f':
                out_ << "\\f";
                break;
            default:
                if (const auto byte = static_cast<unsigned char>(c); byte < 0x20 || byte >= 0x7f)
                {
                    out_ << std::format("\\u{:04x}", static_cast<unsigned>(byte));
                }
                else
                {
                    out_ << c;
                }
                break;
            }
        }
        out_ << '"';
    }

    void json_writer::begin_object()
    {
        write_separator();
        out_ << '{';
        needs_comma_.push_back(false);
    }

    void json_writer::begin_object(std::string_view key)
    {
        write_separator();
        write_key(key);
        out_ << '{';
        needs_comma_.push_back(false);
    }

    void json_writer::end_object()
    {
        const bool had_members = needs_comma_.back();
        needs_comma_.pop_back();
        if (had_members)
        {
            out_ << '\n';
            write_indent();
        }
        out_ << '}';
        if (needs_comma_.empty()) out_ << '\n';
    }

    void json_writer::begin_array(std::string_view key)
    {
        write_separator();
        write_key(key);
        out_ << '[';
        needs_comma_.push_back(false);
    }

    void json_writer::end_array()
    {
        const bool had_members = needs_comma_.back();
        needs_comma_.pop_back();
        if (had_members)
        {
            out_ << '\n';
            write_indent();
        }
        out_ << ']';
    }

    void json_writer::key_string(std::string_view key, std::string_view value)
    {
        write_separator();
        write_key(key);
        write_escaped(value);
    }

    void json_writer::key_bool(std::string_view key, bool value)
    {
        write_separator();
        write_key(key);
        out_ << (value ? "true" : "false");
    }

    void json_writer::key_uint(std::string_view key, uint64_t value)
    {
        write_separator();
        write_key(key);
        out_ << std::format("{}", value);
    }

    void json_writer::key_hex(std::string_view key, uint64_t value)
    {
        write_separator();
        write_key(key);
        write_escaped(std::format("{:#x}", value));
    }
}
