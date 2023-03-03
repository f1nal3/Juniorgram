#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("Channel request procedures [Server][Failed]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    SECTION("Failed request to create a channel")
    {
        Message     invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage, 
            MessageType::ChannelCreateRequest, MessageBody::InvalidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
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

        Sleep(1000);
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

        Sleep(1000);
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

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() == 
            TestObject::MessageResult::InvalidBody);
        testServer->stop();
    }
}
