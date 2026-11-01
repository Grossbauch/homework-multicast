#include <charconv>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "config.hpp"

namespace multicast
{

namespace
{

Tokens split(std::string_view str)
{
    std::vector<std::string_view> tokens{};
    std::size_t start = 0;

    while (start < str.size())
    {
        const std::size_t pos = str.find_first_of(" \t", start);
        if (pos == std::string_view::npos)
        {
            tokens.emplace_back(str.substr(start));
            break;
        }

        if (pos > start)
        {
            tokens.emplace_back(str.substr(start, pos - start));
        }

        start = pos + 1;
    }

    return tokens;
}

int parse_port(std::string_view str)
{
    int port = 0;

    const auto* begin = str.data();
    const auto* end = begin + str.size();
    const auto result = std::from_chars(begin, end, port);

    if (result.ec != std::errc{} || result.ptr != end || port <= 0)
    {
        throw std::out_of_range("invalid port number");
    }

    return port;
}

} // namespace

UdpEntry::UdpEntry(const Tokens& tokens)
{
    if (tokens.size() != 3)
    {
        throw std::runtime_error("can't create UDP client");
    }

    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;

    const std::string ipAddressStr{tokens[1]};
    if (inet_pton(AF_INET, ipAddressStr.c_str(), &address.sin_addr) <= 0)
    {
        throw std::runtime_error("invalid IP address");
    }

    address.sin_port = htons(parse_port(tokens[2]));
}

SerialEntry::SerialEntry(const Tokens& tokens)
{
    if (tokens.size() != 2)
    {
        throw std::runtime_error("can't create serial client");
    }

    device = tokens[1];
}

Config::Config(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error("configuration file does not exists!");
    }

    std::ifstream ifs{path};
    if (!ifs || !ifs.good())
    {
        throw std::runtime_error("failed to open configuration file");
    }

    std::string line{};
    while (std::getline(ifs, line))
    {
        const auto tokens = split(line);
        if (tokens.empty())
        {
            continue;
        }

        if (tokens[0] == "udp")
        {
            udpEntries_.emplace_back(UdpEntry{tokens});
        }
        else if (tokens[0] == "serial")
        {
            serialEntries_.emplace_back(SerialEntry{tokens});
        }
        else
        {
            throw std::runtime_error("unknown client");
        }
    }
}

const Config::UdpEntries& Config::GetUdpEntries() const
{
    return udpEntries_;
}

const Config::SerialEntries& Config::GetSerialEntries() const
{
    return serialEntries_;
}

} // namespace multicast
