#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("TestServerRegistrationRequest [success case]")
{
    TestClient  mockClient;
    auto        testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::RegistrationRequest, MessageBody::ExpensiveBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerLoggingRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::LoginRequest, MessageBody::ExpensiveBody));
    }
    testServerUpdating(testServer);
}
