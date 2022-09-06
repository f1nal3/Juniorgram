#include <catch2/catch.hpp>
#include <TestUtility.hpp>

using namespace TestUtility;

TEST_CASE("Check server ping")
{
    Client     Client;
    testServer serverTest(ServerInfo::Port::test);

    serverTest.start();

    if (bindOfConnectToServer(Client) == true)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::ServerPing));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check default request of server side")
{
    Client     Client;
    testServer serverTest(ServerInfo::Port::test);
    const int16_t failedType = 666;

    serverTest.start();

    if (bindOfConnectToServer(Client) == true)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType(failedType)));
    }
    testServerUpdating(serverTest);
}
