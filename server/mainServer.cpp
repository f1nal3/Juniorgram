#include <network.h>

int main(int argc, char** argv)
{
    network::Server server(6969);
    server.start();

    while (true)
    {
        server.update();
    }

    return 0;
}
