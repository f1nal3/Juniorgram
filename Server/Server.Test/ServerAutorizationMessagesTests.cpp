#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

constexpr bool acceptingConnection = true;

TEST_CASE("Check registration request of server")
{
    Client     mockClient;
    testServer serverTest(getTestingPort(),getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::RegistrationRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check login request of server")
{
    Client     mockClient;
    testServer serverTest(getTestingPort(),getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::LoginRequest));
    }
    testServerUpdating(serverTest);
}
