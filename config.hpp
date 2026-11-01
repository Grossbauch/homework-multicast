#pragma once

#include <filesystem>
#include <string_view>
#include <vector>

#include <netinet/in.h>

namespace multicast
{

using Tokens = std::vector<std::string_view>;

struct UdpEntry
{
    sockaddr_in address;
    explicit UdpEntry(const Tokens& tokens);
};

struct SerialEntry
{
    std::string device;
    explicit SerialEntry(const Tokens& tokens);
};

class Config
{
public:
    using UdpEntries = std::vector<UdpEntry>;
    using SerialEntries = std::vector<SerialEntry>;

    Config(const std::filesystem::path& path);
    const UdpEntries& GetUdpEntries() const;
    const SerialEntries& GetSerialEntries() const;

private:
    UdpEntries udpEntries_;
    SerialEntries serialEntries_;
};

} // namespace multicast
