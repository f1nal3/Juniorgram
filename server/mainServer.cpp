#include <network.h>


class foo
{
    foo(int argc, char** argv) {
    //Count check

    // if (dictionary.find(first_argument))
    _portNumber = parse.SecondParam();
    }

};
int main(int argc, char** argv)
{
    foo(argc, argv);
    network::Server server(foo.getPortNumber());
    server.start();

    while (true)
    {
        server.update();
    }

    return 0;
}
