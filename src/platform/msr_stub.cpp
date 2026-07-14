#include "../msr.hpp"

#if !defined(__linux__) && !defined(_WIN32)

namespace vmx::msr
{
    msr_read_result read_msr(uint32_t, unsigned) noexcept
    {
        return {msr_status::not_implemented, 0};
    }
}

#endif
