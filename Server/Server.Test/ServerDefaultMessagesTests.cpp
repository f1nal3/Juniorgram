#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

TEST_CASE("Check server ping")
{
    Client     Client;
    testServer serverTest(ServerInfo::Port::test);
    bool       acceptingConnection = true;

    serverTest.start();

    if (bindOfConnectToServer(Client) == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::ServerPing));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check default request of server side")
{
    Client     Client;
    testServer serverTest(ServerInfo::Port::test);

    constexpr auto failedType         = int16_t(666);
    bool          acceptingConnection = true;

    serverTest.start();

    if (bindOfConnectToServer(Client) == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType(failedType)));
    }
    testServerUpdating(serverTest);
}
