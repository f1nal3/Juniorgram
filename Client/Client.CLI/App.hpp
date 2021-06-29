#include <Client.hpp>
#include <string>

class App
{
private:
    const std::string address = "127.0.0.1";
	
    const std::uint16_t port            = 65003;
    Network::Client client;
>>>>>>> 7175ced (Small changes ._.)

public:
    App(/* args */);
    ~App();

    Network::Client* shell();
    bool loop();
};
