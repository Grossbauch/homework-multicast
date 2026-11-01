#include <cerrno>
#include <cstddef>
#include <stdexcept>
#include <string_view>

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include "serial.hpp"

namespace multicast
{
namespace serial
{

Device::Device(std::string_view path)
{
    fd_ = open(path.data(), O_WRONLY | O_NOCTTY | O_SYNC);
    if (fd_ < 0)
    {
        throw std::runtime_error("failed to open serial device");
    }

    termios tty{};
    if (tcgetattr(fd_, &tty) != 0)
    {
        close(fd_);
        throw std::runtime_error("tcgetattr failed");
    }

    if (cfsetospeed(&tty, B9600) != 0)
    {
        close(fd_);
        throw std::runtime_error("cfsetospeed failed");
    }
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_cflag |= CLOCAL | CREAD;
    tty.c_cflag &= ~(PARENB | CSTOPB | CRTSCTS);

    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_iflag = 0;

    if (tcsetattr(fd_, TCSANOW, &tty) != 0)
    {
        close(fd_);
        throw std::runtime_error("tcsetattr failed");
    }
}

Device::~Device() noexcept
{
    close(fd_);
}

int Device::Handler() const noexcept
{
    return fd_;
}

bool send(const Device& device, std::string_view message)
{
    const auto* data = message.data();
    std::size_t remaining = message.size();

    while (remaining > 0)
    {
        const ssize_t written = write(device.Handler(), data, remaining);
        if (written < 0)
        {
            if (errno != EINTR)
            {
                return false;
            }
            continue;
        }

        data += written;
        remaining -= static_cast<std::size_t>(written);
    }

    return true;
}

} // namespace serial
} // namespace multicast
