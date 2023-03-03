#include <catch2/catch.hpp>

#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("Message request procedures [Server][Failed]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    SECTION("Failed message history request")
    {
        Message invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage, 
            MessageType::MessageHistoryRequest, MessageBody::InvalidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::InvalidBody);
        testServer->stop();
    }

    SECTION("Failed reply message history request")
    {
        Message invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage, 
            MessageType::ReplyHistoryRequest, MessageBody::InvalidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::InvalidBody);
        testServer->stop();
    }

    SECTION("Failed direct message request")
    {
        Message invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage, 
            MessageType::DirectMessageCreateRequest, MessageBody::InvalidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::InvalidBody);
        testServer->stop();
    }

    SECTION("Failed message store request")
    {
        Message invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage, 
            MessageType::MessageStoreRequest, MessageBody::InvalidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::InvalidBody);
        testServer->stop();
    }

    SECTION("Failed reply message store request")
    {
        Message invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage, 
            MessageType::ReplyStoreRequest, MessageBody::InvalidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::InvalidBody);
        testServer->stop();
    }

    SECTION("Failed message reaction request")
    {
        Message invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage, 
            MessageType::MessageReactionRequest, MessageBody::InvalidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::InvalidBody);
        testServer->stop();
    }

    SECTION("Failed message edit request")
    {
        Message invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage, 
            MessageType::MessageEditRequest, MessageBody::InvalidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::InvalidBody);
        testServer->stop();
    }

    SECTION("Failed message delete request")
    {
        Message invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage, 
            MessageType::MessageDeleteRequest, MessageBody::InvalidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::InvalidBody);
        testServer->stop();
    }
}
