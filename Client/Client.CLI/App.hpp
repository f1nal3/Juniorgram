#include <Client.hpp>
#include <string>

class App
{
private:
    Network::Client client;

public:
    App(/* args */);
    ~App();

    Network::Client* shell();
    bool loop();
};
