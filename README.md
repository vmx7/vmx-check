vmx-check v0.1.1  -  intel vt-x capability dumper
-------------------------------------------------

reads cpuid + vmx msrs, prints a decoded capability tree. detects vendor,
family/model/stepping, vt-x/amd-v support, and the running-under-hypervisor
bit with its vendor string. with root (linux) it also reads and decodes the
vmx capability msrs: ia32_vmx_basic, the pin/proc/secondary-based controls,
the vm-exit and vm-entry controls, and ept/vpid caps. without msr access it
prints the cpuid section plus a one-line reason and exits cleanly.

build:
    cmake -S . -B build && cmake --build build

run:
    ./build/vmx-check          # linux: sudo ./build/vmx-check for msr sections

flags:
    --json         emit json instead of text
    --no-color     disable ansi colors
    --help         print this help

refs:
    intel sdm vol.3 appendix a
    intel sdm vol.2a cpuid
    amd apm vol.3 cpuid fn8000_0001h
