#include "Network/Server.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <stdexcept>
#include <map>
#include <string>

class ArgumentParser
{
private:
    std::map<std::string, int32_t> arguments;
    const std::array<std::string, 2> validKeys = {"-p", "-d"};

public:
    ArgumentParser()                      = delete;
    ArgumentParser(const ArgumentParser&) = delete;

    explicit ArgumentParser(int argc, char** argv)
    {
        if (argc % 2 == 0 || argc <= 1)
        {
            throw std::runtime_error("Bad arguments amount");
        }

        for (int i = 1; i < argc; i += 2)
        {
            if (isKeyValid(argv[i]) && !isMapContainingKey(argv[i]))
            {
                arguments.emplace(argv[i], atoi(argv[i + 1]));
            }
            else
            {
                throw std::runtime_error("Arguments have identical keys or keys are unvalid");
            }
        }

        realBD = !isMapContainingKey("-d");
    }

    uint16_t getPort() const
    {
        size_t indexValidKey = 0;
        for (size_t i = 0; i < validKeys.size(); i++)
        {
            if (isMapContainingKey(validKeys[i]))
            {
                indexValidKey = i;
                break;
            }
        }

        auto port = arguments.find(validKeys[indexValidKey])->second;

        if (port < std::numeric_limits<uint16_t>::min() ||
            port > std::numeric_limits<uint16_t>::max())
        {
            throw std::runtime_error("Port value is unvalid");
        }

        return static_cast<uint16_t>(port);
    }

    bool getInd() const noexcept { return realBD; }

private:
    bool realBD;

    bool isKeyValid(const std::string& incomingKey) const noexcept
    {
        return std::any_of(validKeys.cbegin(), validKeys.cend(),
                           [&](const std::string& validKey) { return incomingKey == validKey; });
    }

    bool isMapContainingKey(const std::string& incomingKey) const noexcept
    {
        return std::any_of(arguments.cbegin(), arguments.cend(),
                           [&](const std::pair<std::string, std::int32_t>& existingArg) {
                               return incomingKey == existingArg.first;
                           });
    }
};

int main(int argc, char** argv)
{
    try
    {
        ArgumentParser parser(argc, argv);

        network::Server server(parser.getPort());
        server.start();

        while (true)
        {
            server.update();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
