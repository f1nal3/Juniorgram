#include <network.h>

int main(int argc, char** argv)
{
    network::Server server(60000);
    server.start();

    while (true)
    {
        server.update();
    }

    return 0;
}
