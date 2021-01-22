#include <network.h>

int main() {
    network::Server server(60000);
    server.start();

    while (true)
    {
        server.update();
    }

    return 0;
}
