#include <catch2/catch.hpp>

#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("TestServerFailedHistoryRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message invalidMessage;
    const auto& messageInstance = makeMessage(invalidMessage, 
        MessageType::MessageHistoryRequest, MessageBody::InvalidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerReplyingFailedHistoryRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message invalidMessage;
    const auto& messageInstance = makeMessage(invalidMessage, 
        MessageType::ReplyHistoryRequest, MessageBody::InvalidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerCreatingFailedDirectMessage [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message invalidMessage;
    const auto& messageInstance = makeMessage(invalidMessage, 
        MessageType::DirectMessageCreateRequest, MessageBody::InvalidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerMessageFailedStoreRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message invalidMessage;
    const auto& messageInstance = makeMessage(invalidMessage,
        MessageType::MessageStoreRequest, MessageBody::InvalidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerReplyingFailedStoreRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message invalidMessage;
    const auto& messageInstance = makeMessage(invalidMessage,
        MessageType::ReplyStoreRequest, MessageBody::InvalidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerFailedReactionRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message invalidMessage;
    const auto& messageInstance = makeMessage(invalidMessage,
        MessageType::MessageReactionRequest, MessageBody::InvalidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerMessageFailedEditingRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message invalidMessage;
    const auto& messageInstance = makeMessage(invalidMessage,
        MessageType::MessageEditRequest, MessageBody::InvalidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerMessageFailedDeletingRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message invalidMessage;
    const auto& messageInstance = makeMessage(invalidMessage,
        MessageType::MessageDeleteRequest, MessageBody::InvalidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    testServer->update();
    testServer->stop();
}
