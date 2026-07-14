cmake_minimum_required(VERSION 3.20)

project(vmx_check LANGUAGES CXX)

add_executable(vmx-check
    src/main.cpp
    src/printer.cpp
    src/vmx/ia32_vmx_basic.cpp
    src/vmx/ia32_vmx_pinbased_ctls.cpp
    src/vmx/ia32_vmx_procbased_ctls.cpp
    src/vmx/ia32_vmx_procbased_ctls2.cpp)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_sources(vmx-check PRIVATE src/platform/msr_linux.cpp)
elseif (WIN32)
    target_sources(vmx-check PRIVATE src/platform/msr_windows.cpp)
else ()
    target_sources(vmx-check PRIVATE src/platform/msr_stub.cpp)
endif ()

target_compile_features(vmx-check PRIVATE cxx_std_20)

if (MSVC)
    target_compile_options(vmx-check PRIVATE /W4 /WX /permissive-)
else ()
    target_compile_options(vmx-check PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif ()
