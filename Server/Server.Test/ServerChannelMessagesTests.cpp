#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("Channel request procedures [Server][Success]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    SECTION("Successful request to create a channel")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::ChannelCreateRequest, MessageBody::ValidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::Success);
        testServer->stop();
    }

    SECTION("Successful request subscription to channel")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage,
            MessageType::ChannelSubscribeRequest, MessageBody::ValidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::Success);
        testServer->stop();
    }

    SECTION("Successful request subscription list from channel ")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage,
            MessageType::ChannelSubscriptionListRequest, MessageBody::ValidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::Success);
        testServer->stop();
    }

    SECTION("Successful request channel list")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage,
            MessageType::ChannelListRequest, MessageBody::ValidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::Success);
        testServer->stop();
    }

    SECTION("Successful request to leave from channel")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::ChannelLeaveRequest, MessageBody::ValidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::Success);
        testServer->stop();
    }

    SECTION("Successful request to delete a channel")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage,
            MessageType::ChannelDeleteRequest, MessageBody::ValidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::Success);
        testServer->stop();
    }
}
