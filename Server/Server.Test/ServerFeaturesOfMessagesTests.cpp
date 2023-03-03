#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("Message request procedures [Server][Success]")
{
	auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    SECTION("Successful request to receive all messages")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage,
            MessageType::MessageAll, MessageBody::ValidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::Success);
        testServer->stop();
    }

    SECTION("Successful message history request")
    {
        Message validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::MessageHistoryRequest, MessageBody::ValidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::Success);
        testServer->stop();
    }

    SECTION("Successful reply message history request")
    {
        Message validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::ReplyHistoryRequest, MessageBody::ValidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::Success);
        testServer->stop();
    }

    SECTION("Successful direct message request")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::DirectMessageCreateRequest, MessageBody::ValidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::Success);
        testServer->stop();
    }

    SECTION("Successful message store request")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::MessageStoreRequest, MessageBody::ValidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::Success);
        testServer->stop();
    }

    SECTION("Successful reply message store request")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::ReplyStoreRequest, MessageBody::ValidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::Success);
        testServer->stop();
    }

    SECTION("Successful message reaction request")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::MessageReactionRequest, MessageBody::ValidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::Success);
        testServer->stop();
    }

    SECTION("Successful message edit request")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::MessageEditRequest, MessageBody::ValidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::Success);
        testServer->stop();
    }

    SECTION("Successful message delete request")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::MessageDeleteRequest, MessageBody::ValidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::Success);
        testServer->stop();
    }
}
