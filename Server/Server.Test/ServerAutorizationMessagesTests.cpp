#include <catch2/catch.hpp>
#include <TestUtility.hpp>

using namespace TestUtility;

TEST_CASE("Check registration request of server")
{
    Client     Client;
    testServer serverTest(ServerInfo::Port::test);

    serverTest.start();

    if (bindOfConnectToServer(Client) == true)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::RegistrationRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check login request of server")
{
    Client     Client;
    testServer serverTest(ServerInfo::Port::test);

    serverTest.start();

    if (bindOfConnectToServer(Client) == true)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::LoginRequest));
    }
    testServerUpdating(serverTest);
}
