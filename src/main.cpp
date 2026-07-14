#include <cstdint>
#include <format>
#include <iostream>
#include <string>

#include "cpuid.hpp"

namespace
{
    struct cpu_signature
    {
        uint32_t family;
        uint32_t model;
        uint32_t stepping;
    };

    // sdm vol.2a, cpuid eax=01h, version information in eax
    cpu_signature decode_signature(uint32_t eax)
    {
        const uint32_t stepping = eax & 0xf;
        const uint32_t base_model = (eax >> 4) & 0xf;
        const uint32_t base_family = (eax >> 8) & 0xf;
        const uint32_t extended_model = (eax >> 16) & 0xf;
        const uint32_t extended_family = (eax >> 20) & 0xff;

        cpu_signature sig{base_family, base_model, stepping};
        if (base_family == 0x0f) sig.family += extended_family;
        if (base_family == 0x06 || base_family == 0x0f) sig.model += extended_model << 4;
        return sig;
    }

    std::string cpu_vendor()
    {
        const auto regs = vmx::read_cpuid(0);
        std::string vendor{};
        for (const auto reg : {regs.ebx, regs.edx, regs.ecx})
        {
            for (int shift = 0; shift < 32; shift += 8)
            {
                vendor.push_back(static_cast<char>(reg >> shift));
            }
        }
        return vendor;
    }
}

int main()
{
    const std::string vendor = cpu_vendor();
    std::cout << "vendor: " << vendor << '\n';

    const auto leaf1 = vmx::read_cpuid(1);
    const auto sig = decode_signature(leaf1.eax);
    std::cout << std::format("family/model/stepping: {:02x}_{:02x}_{}\n", sig.family, sig.model,
                             sig.stepping);

    // sdm vol.3 24.6, cpuid eax=01h, ecx bit 5
    constexpr uint32_t vmx_feature_bit = 1u << 5;
    const bool vmx_supported = (leaf1.ecx & vmx_feature_bit) != 0;
    std::cout << "vmx (intel vt-x): " << (vmx_supported ? "supported" : "not supported") << '\n';

    if (vendor == "AuthenticAMD")
    {
        // amd apm vol.3, cpuid fn8000_0001h, ecx bit 2
        constexpr uint32_t svm_feature_bit = 1u << 2;
        const bool svm_supported = (vmx::read_cpuid(0x80000001).ecx & svm_feature_bit) != 0;
        std::cout << "svm (amd-v): " << (svm_supported ? "supported" : "not supported") << '\n';
    }
    else
    {
        std::cout << "svm (amd-v): n/a\n";
    }
    return 0;
}
