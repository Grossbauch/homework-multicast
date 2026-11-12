#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <netinet/in.h>

#include "config.hpp"
#include "serial.hpp"
#include "udp.hpp"

namespace
{

void print_usage(std::string_view basename)
{
    std::cout << "Usage: " << basename << " <CONFIG_FILE>\n";
}

void print_prompt()
{
    std::cout << "Enter message: ";
}

class Client
{
public:
    virtual ~Client() = default;
    virtual bool Send(std::string_view message) = 0;
};

class UdpClient final : public Client
{
public:
    UdpClient(const multicast::udp::Socket& socket, const sockaddr_in& address)
        : socket_(socket), address_(address)
    {
    }

    bool Send(std::string_view message) override
    {
        return multicast::udp::send(socket_, address_, message);
    }

private:
    const multicast::udp::Socket& socket_;
    sockaddr_in address_;
};

class SerialClient final : public Client
{
public:
    explicit SerialClient(std::string_view path) : device_(path)
    {
    }

    bool Send(std::string_view message) override
    {
        return multicast::serial::send(device_, message);
    }

private:
    multicast::serial::Device device_;
};

} // namespace

int main(int argc, char* argv[])
{
    if (argc == 2 && std::string_view(argv[1]) == "-h")
    {
        print_usage(argv[0]);
        return EXIT_SUCCESS;
    }

    if (argc < 2)
    {
        std::cerr << "Not enough arguments!\n";
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    try
    {
        using namespace multicast;
        Config config{argv[1]};
        udp::Socket socket{};

        std::vector<std::unique_ptr<Client>> clients;

        for (const auto& entry : config.GetUdpEntries())
        {
            clients.push_back(
                std::make_unique<UdpClient>(socket, entry.address));
        }

        for (const auto& entry : config.GetSerialEntries())
        {
            clients.push_back(std::make_unique<SerialClient>(entry.device));
        }

        std::string input{};
        while (print_prompt(), std::getline(std::cin, input))
        {
            input.push_back('\n');

            for (const auto& client : clients)
            {
                if (!client->Send(input))
                {
                    std::cerr << "Failed to send message to a client\n";
                }
            }
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& err)
    {
        std::cerr << "Multicast failure: " << err.what() << '\n';
    }
    return EXIT_FAILURE;
}
