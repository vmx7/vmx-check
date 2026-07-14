#pragma once

#include <cstdint>

namespace vmx::msr
{
    enum class msr_status
    {
        ok,
        not_supported,
        permission_denied,
        io_error,
        not_implemented
    };

    struct msr_read_result
    {
        msr_status status;
        uint64_t value;
    };

    [[nodiscard]] msr_read_result read_msr(uint32_t msr_index, unsigned cpu = 0) noexcept;
}
