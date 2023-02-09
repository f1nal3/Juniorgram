#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

TEST_CASE("TestServerRegistrationRequest [success case]")
{
    Client      mockClient;
    auto        testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::RegistrationRequest, StrongBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerRegistrationFailedRequest [success case]")
{
    Client      mockClient;
    auto        testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::RegistrationRequest, PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerLoggingRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::LoginRequest, StrongBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerLoggingFailedRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::LoginRequest, PoorBody));
    }
    testServerUpdating(testServer);
}
