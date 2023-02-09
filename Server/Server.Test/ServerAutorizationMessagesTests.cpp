#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;
using TestUtility::TypeOfMessageBody;

TEST_CASE("TestServerRegistrationRequest [success case]")
{
    Client      mockClient;
    auto        testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::RegistrationRequest, TypeOfMessageBody::StrongBody));
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
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::RegistrationRequest, TypeOfMessageBody::PoorBody));
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
            MessageType::LoginRequest, TypeOfMessageBody::StrongBody));
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
            MessageType::LoginRequest, TypeOfMessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}
