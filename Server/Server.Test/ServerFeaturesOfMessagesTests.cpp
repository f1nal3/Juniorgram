#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;
using TestUtility::TypeOfMessageBody;

TEST_CASE("TestServerGivingAllMessagesRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::MessageAll, TypeOfMessageBody::StrongBody));
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
            MessageType::MessageHistoryRequest, TypeOfMessageBody::StrongBody));
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
            MessageType::MessageHistoryRequest, TypeOfMessageBody::PoorBody));
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
            MessageType::ReplyHistoryRequest, TypeOfMessageBody::StrongBody));
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
            MessageType::ReplyHistoryRequest, TypeOfMessageBody::PoorBody));
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
            MessageType::DirectMessageCreateRequest, TypeOfMessageBody::StrongBody));
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
            MessageType::DirectMessageCreateRequest, TypeOfMessageBody::PoorBody));
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
            MessageType::MessageStoreRequest, TypeOfMessageBody::StrongBody));
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
            MessageType::MessageStoreRequest, TypeOfMessageBody::PoorBody));
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
            MessageType::ReplyStoreRequest, TypeOfMessageBody::StrongBody));
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
            MessageType::ReplyStoreRequest, TypeOfMessageBody::PoorBody));
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
            MessageType::MessageReactionRequest, TypeOfMessageBody::StrongBody));
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
            MessageType::MessageReactionRequest, TypeOfMessageBody::PoorBody));
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
            MessageType::MessageEditRequest, TypeOfMessageBody::StrongBody));
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
            MessageType::MessageEditRequest, TypeOfMessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerMessageDeletingRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::MessageDeleteRequest, TypeOfMessageBody::StrongBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerMessageDeletingFailedRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::MessageDeleteRequest, TypeOfMessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}
