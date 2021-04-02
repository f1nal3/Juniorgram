#include <Client.hpp>
#include <string>

class App
{
private:
    const std::string address = "104.40.239.183";
    const std::uint16_t port            = 65001;
    Network::Client client;

public:
    App(/* args */);
    ~App();

    Network::Client* shell();
    bool loop();
};
