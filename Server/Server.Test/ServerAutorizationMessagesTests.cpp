#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("TestServerRegistrationRequest [success case]")
{
    Client      mockClient;
    auto        testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::RegistrationRequest, MessageBody::ExpensiveBody));
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
            MessageType::LoginRequest, MessageBody::ExpensiveBody));
    }
    testServerUpdating(testServer);
}
