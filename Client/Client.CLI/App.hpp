#include <Client.hpp>
#include <string>

class App
{
private:
    const std::string address = "127.0.0.1";
    const std::uint16_t port            = 65001;
    Network::Client client;

public:
    App(/* args */);
    ~App();

    Network::Client* shell();
    bool loop();
};
