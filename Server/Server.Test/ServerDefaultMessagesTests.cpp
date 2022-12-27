#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

 constexpr bool acceptingConnection = true;

TEST_CASE("Check server ping")
{
    Client     Client;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(Client,getTestingAddress(),getTestingPort()) 
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::ServerPing));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check default request of server side")
{
    Client     Client;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    constexpr auto failedType         = int16_t(666);

    serverTest.start();

    if (bindOfConnectToServer(Client, getTestingAddress(), getTestingPort())
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType(failedType)));
    }
    testServerUpdating(serverTest);
}
