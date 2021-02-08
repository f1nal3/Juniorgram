#include <network.h>

#include <string>

class App
{
private:
    const std::string address = "127.0.0.1";
    const int port            = 60000;
    network::Client client;

public:
    App(/* args */);
    ~App();

    network::Client* shell();
    bool loop();
};
