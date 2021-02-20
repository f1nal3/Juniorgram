#include <network.h>
#include <map>

class ArgumentParser
{
private:
    std::map<std::string, std::int32_t> arguments;
    const std::array<std::string, 1> validKeys = {"-p"};

public:
    ArgumentParser()                      = delete;
    ArgumentParser(const ArgumentParser&) = delete;

    explicit ArgumentParser(int argc, char** argv)
    {
        /*
        if (argv[0] != path_to_project)
        {
            throw std::exception("First argument must be a project's path");
        }
        */

        if (argc % 2 == 0 && argc <= 1)
        {
            throw std::exception("Bad arguments amount");
        }

        for (int i = 1; i < argc; i += 2)
        {
            if (isKeyValid(argv[i]) && !mapContainsKey(argv[i]))
            {
                arguments.emplace(argv[i], atoi(argv[i + 1]));
            }
            else
            {
                throw std::exception("Identical keys were found in the arguments");
            }
        }
    }

    uint16_t getPort() const
    {
        auto port = arguments.find("-p")->second;

        if (port <= 0 || port > UINT16_MAX)
        {
            throw std::exception("Port value is not valid");
        }

        return (uint16_t)port;
    }

private:
    bool isKeyValid(const std::string& incomingArg) const noexcept
    {
        return std::any_of(
            validKeys.cbegin(), validKeys.cend(),
            [&](const std::string& validKey) { return incomingArg == validKey; });
    }

    bool mapContainsKey(const std::string& incomingArg) const noexcept
    {
        return std::any_of(arguments.cbegin(), arguments.cend(),
                           [&](const std::pair<std::string, std::int32_t>& existingArg) {
                               return incomingArg == existingArg.first;
                           });
    }
};

suppressWarning(4100, -Wunused - parameter)
int main(int argc, char** argv)
{
    argv[1] = "-p";
    argv[2] = "6969";
    argc    = 3;

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
    catch (const std::exception& error)
    {
        std::cerr << error.what() << std::endl;
    }

    return 0;
}
restoreWarning