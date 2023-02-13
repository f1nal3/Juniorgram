#include <catch2/catch.hpp>

#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("TestServerHistoryFailedRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::MessageHistoryRequest, MessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerReplyingHistoryFailedRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ReplyHistoryRequest, MessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerCreatingFailedDirectMessage [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::DirectMessageCreateRequest, MessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerMessageStoreFailedRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::MessageStoreRequest, MessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerReplyingStoreFailedRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::ReplyStoreRequest, MessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerReactionFailedRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::MessageReactionRequest, MessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerMessageEditingFailedRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::MessageEditRequest, MessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerMessageDeletingFailedRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::MessageDeleteRequest, MessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}
