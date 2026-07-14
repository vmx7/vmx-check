#include <iostream>
#include <string>

#include "cpuid.hpp"

namespace
{
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
    std::cout << "vendor: " << cpu_vendor() << '\n';
    return 0;
}
