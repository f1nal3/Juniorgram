#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"
#include "Utilities/WaitFor.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("Channel request procedures [Server][Success]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    WaitFor waiter(milliseconds(1000));

    SECTION("Successful request to create a channel")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::ChannelCreateRequest, MessageBody::ValidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        waiter.wait();
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

        waiter.wait();
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

        waiter.wait();
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

        waiter.wait();
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

        waiter.wait();
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

        waiter.wait();
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::Success);
        testServer->stop();
    }
}

TEST_CASE("Channel request procedures [Server][Failed]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    WaitFor waiter(milliseconds(1000));

    SECTION("Failed request to create a channel")
    {
        Message     invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage, 
            MessageType::ChannelCreateRequest, MessageBody::InvalidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        waiter.wait();
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::InvalidBody);
        testServer->stop();
    }

    SECTION("Failed request subscription to channel")
    {
        Message     invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage, 
            MessageType::ChannelSubscribeRequest, MessageBody::InvalidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        waiter.wait();
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::InvalidBody);
        testServer->stop();
    }
    
    SECTION("Failed request to leave from channel")
    {
        Message     invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage, 
            MessageType::ChannelLeaveRequest, MessageBody::InvalidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        waiter.wait();
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::InvalidBody);
        testServer->stop();
    }

    SECTION("Failed request to delete a channel")
    {
        Message     invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage, 
            MessageType::ChannelDeleteRequest, MessageBody::InvalidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        waiter.wait();
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::InvalidBody);
        testServer->stop();
    }
}
