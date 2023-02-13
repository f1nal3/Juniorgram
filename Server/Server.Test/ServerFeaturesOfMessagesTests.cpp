#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("TestServerGivingAllMessagesRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::MessageAll, MessageBody::ExpensiveBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerHistoryRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::MessageHistoryRequest, MessageBody::ExpensiveBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerReplyingHistoryRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ReplyHistoryRequest, MessageBody::ExpensiveBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerCreatingDirectMessage [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::DirectMessageCreateRequest, MessageBody::ExpensiveBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerMessageStoreRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::MessageStoreRequest, MessageBody::ExpensiveBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerReplyingStoreRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::ReplyStoreRequest, MessageBody::ExpensiveBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerReactionRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::MessageReactionRequest, MessageBody::ExpensiveBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerMessageEditingRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::MessageEditRequest, MessageBody::ExpensiveBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerMessageDeletingRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::MessageDeleteRequest, MessageBody::ExpensiveBody));
    }
    testServerUpdating(testServer);
}
