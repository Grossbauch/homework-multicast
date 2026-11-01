#include <cstdlib>
#include <iostream>

#include "config.hpp"
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

        for ([[maybe_unused]] const auto& entry : config.GetUdpEntries())
        {
            // TODO: добавьте клиента (используйте объект socket)
        }

        for ([[maybe_unused]] const auto& entry : config.GetSerialEntries())
        {
            // TODO: добавьте клиента (для каждого клиента создавайте
            // новые объекты serial::Device)
        }

        std::string input{};
        while (print_prompt(), std::getline(std::cin, input))
        {
            input.push_back('\n');

            // TODO: отправьте всем клиентам
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& err)
    {
        std::cerr << "Multicast failure: " << err.what() << '\n';
    }
    return EXIT_FAILURE;
}
