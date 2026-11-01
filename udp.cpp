#include <stdexcept>
#include <string_view>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "udp.hpp"

namespace multicast
{
namespace udp
{

Socket::Socket()
{
    fd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd_ < 0)
    {
        throw std::runtime_error("failed to open UDP socket");
    }
}

int Socket::Handler() const noexcept
{
    return fd_;
}

Socket::~Socket() noexcept
{
    close(fd_);
}

bool send(const Socket& socket, const sockaddr_in& address,
    std::string_view message)
{
    const ssize_t sent =
        sendto(socket.Handler(), message.data(), message.size(), 0,
            reinterpret_cast<const sockaddr*>(&address), sizeof(address));

    return sent >= 0;
}

} // namespace udp
} // namespace multicast
