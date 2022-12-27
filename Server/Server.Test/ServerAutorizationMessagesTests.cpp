#include <catch2/catch.hpp>
#include "ServerUtility/TestUtility.hpp"

using namespace TestUtility;

constexpr bool acceptingConnection = true;

TEST_CASE("Check registration request of server")
{
    Client     Client;
    testServer serverTest(getTestingPort(),getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(Client, getTestingAddress(), getTestingPort()) == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::RegistrationRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check login request of server")
{
    Client     Client;
    testServer serverTest(getTestingPort(),getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(Client, getTestingAddress(), getTestingPort()) == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::LoginRequest));
    }
    testServerUpdating(serverTest);
}
