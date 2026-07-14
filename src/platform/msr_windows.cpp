#include "../msr.hpp"

#if defined(_WIN32)

namespace vmx::msr
{
    // reading msrs on windows requires a signed kernel driver
    msr_read_result read_msr(uint32_t, unsigned) noexcept
    {
        return {msr_status::not_implemented, 0};
    }
}

#endif
