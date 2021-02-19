#include <network.h>

int main()
{
    network::Server server(6969);
    server.start();

    while (true)
    {
        server.update();
    }

    return 0;
}
