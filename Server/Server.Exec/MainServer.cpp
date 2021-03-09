#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "Network/Server.hpp"

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
            if (isKeyValid(argv[i]) && !doMapContainKey(argv[i]))
            {
                arguments.emplace(argv[i], atoi(argv[i + 1]));
            }
            else
            {
                throw std::runtime_error("Arguments have identical keys or keys are unvalid");
            }
        }

        realBD = !doMapContainKey("-d");
    }

    uint16_t getPort() const
    {
        std::string indexValidKey;
        for (auto&& validKey : validKeys)
        {
            if (doMapContainKey(validKey))
            {
                indexValidKey = validKey;
                break;
            }
        }

        auto port = arguments.find(indexValidKey)->second;

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
        return std::find(validKeys.begin(), validKeys.end(), incomingKey) != validKeys.end();
    }

    bool doMapContainKey(const std::string& incomingKey) const noexcept
    {
        return arguments.find(incomingKey) != arguments.end();
    }

    bool isInteger(const std::string& s)
    {
        for (auto i : s)
            if ((i < '0') || (i > '9')) return false;
        return true;
    }

    std::string trim(const char* row)
    {
        std::string newRow = row;
        while (newRow[0] == ' ') newRow.erase(newRow.begin());
        while (newRow[newRow.size() - 1] == ' ') newRow.erase(newRow.end() - 1);
        return newRow;
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
