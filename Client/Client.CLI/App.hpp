#include <Client.hpp>
#include <string>

#include <Utility.Static/TokenHolder.hpp>
#include <Utility.Static/KeyHolder.hpp>
#include <Utility.Static/Cryptography.hpp>

class App
{
private:
    const std::string address = "127.0.0.1";
	
    const std::uint16_t port            = 65000;
    Network::Client client;

public:
    App(/* args */);
    ~App();

    Network::Client* shell();
    bool loop();
};
