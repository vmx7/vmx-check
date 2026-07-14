#include "../msr.hpp"

#if defined(__linux__)

#include <cerrno>
#include <fcntl.h>
#include <format>
#include <string>
#include <unistd.h>

namespace vmx::msr
{
    msr_read_result read_msr(uint32_t msr_index, unsigned cpu) noexcept
    {
        const std::string path = std::format("/dev/cpu/{}/msr", cpu);
        const int fd = ::open(path.c_str(), O_RDONLY);
        if (fd < 0)
        {
            switch (errno)
            {
            case EACCES:
            case EPERM:
                return {msr_status::permission_denied, 0};
            case ENOENT:
                return {msr_status::not_supported, 0};
            default:
                return {msr_status::io_error, 0};
            }
        }

        uint64_t value = 0;
        const ssize_t got = ::pread(fd, &value, sizeof(value), static_cast<off_t>(msr_index));
        const int read_errno = errno;
        ::close(fd);

        if (got != static_cast<ssize_t>(sizeof(value)))
        {
            if (read_errno == EACCES || read_errno == EPERM)
            {
                return {msr_status::permission_denied, 0};
            }
            return {msr_status::io_error, 0};
        }
        return {msr_status::ok, value};
    }
}

#endif
