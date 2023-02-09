#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

TEST_CASE("TestServerGivingAllMessagesRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::MessageAll, StrongBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerHistoryRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::MessageHistoryRequest, StrongBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerHistoryFailedRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::MessageHistoryRequest, PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerReplyingHistoryRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ReplyHistoryRequest , StrongBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerReplyingHistoryFailedRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ReplyHistoryRequest , PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerCreatingDirectMessage [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::DirectMessageCreateRequest, StrongBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerCreatingFailedDirectMessage [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::DirectMessageCreateRequest, PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerMessageStoreRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::MessageStoreRequest, StrongBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerMessageStoreFailedRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::MessageStoreRequest, PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerReplyingStoreRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::ReplyStoreRequest, StrongBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerReplyingStoreFailedRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::ReplyStoreRequest, PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerReactionRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::MessageReactionRequest, StrongBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerReactionFailedRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::MessageReactionRequest, PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerMessageEditingRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::MessageEditRequest, StrongBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerMessageEditingFailedRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::MessageEditRequest, PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerMessageDeletingRequest [success case]")
{
    Client     mockClient;
    auto   testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::MessageDeleteRequest, StrongBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerMessageDeletingFailedRequest [success case]")
{
    Client     mockClient;
    auto   testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::MessageDeleteRequest, PoorBody));
    }
    testServerUpdating(testServer);
}
