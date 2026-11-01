#pragma once

#include <string_view>

namespace multicast
{
namespace serial
{

class Device
{
public:
    explicit Device(std::string_view path);
    Device(const Device&) = delete;
    Device(Device&&) = delete;
    Device& operator=(const Device&) = delete;
    Device& operator=(Device&&) = delete;
    ~Device() noexcept;
    int Handler() const noexcept;

private:
    int fd_;
};

bool send(const Device& device, std::string_view message);

} // namespace serial
} // namespace multicast
