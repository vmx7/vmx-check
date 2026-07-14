#pragma once

#include <cstdint>

#if defined(_MSC_VER)
#include <array>
#include <intrin.h>
#else
#include <cpuid.h>
#endif

namespace vmx
{
    struct cpuid_regs
    {
        uint32_t eax, ebx, ecx, edx;
    };

    [[nodiscard]] inline cpuid_regs read_cpuid(uint32_t leaf, uint32_t subleaf = 0) noexcept
    {
#if defined(_MSC_VER)
        std::array<int, 4> data{};
        __cpuidex(data.data(), static_cast<int>(leaf), static_cast<int>(subleaf));
        return {static_cast<uint32_t>(data[0]), static_cast<uint32_t>(data[1]),
                static_cast<uint32_t>(data[2]), static_cast<uint32_t>(data[3])};
#else
        cpuid_regs regs{};
        __cpuid_count(leaf, subleaf, regs.eax, regs.ebx, regs.ecx, regs.edx);
        return regs;
#endif
    }
}
