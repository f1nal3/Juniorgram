#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("TestServerRegistrationFailedRequest [success case]")
{
    TestClient  mockClient;
    auto        testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::RegistrationRequest, MessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerLoggingFailedRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::LoginRequest, MessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}
