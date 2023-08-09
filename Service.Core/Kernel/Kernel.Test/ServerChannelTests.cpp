#include <catch2/catch.hpp>
#include "TestUtils/TestUtility.hpp"
#include "TestUtils/WaitForTime.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("Channel request procedures", "[Kernel][Success]")
{
    auto testKernel = makeTestKernel();
    testKernel->start();

    TestClient client;
    client.connectToKernel(TestKernelInfo::Address::local, TestKernelInfo::Port::test);

    SECTION("Successful request to create a channel")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::ChannelCreateRequest, MessageBody::ValidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::Success);
        testKernel->stop();
    }

    SECTION("Successful request subscription to channel")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage,
            MessageType::ChannelSubscribeRequest, MessageBody::ValidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::Success);
        testKernel->stop();
    }

    SECTION("Successful request subscription list from channel ")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage,
            MessageType::ChannelSubscriptionListRequest, MessageBody::ValidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::Success);
        testKernel->stop();
    }

    SECTION("Successful request channel list")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage,
            MessageType::ChannelListRequest, MessageBody::ValidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::Success);
        testKernel->stop();
    }

    SECTION("Successful request to leave from channel")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::ChannelLeaveRequest, MessageBody::ValidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::Success);
        testKernel->stop();
    }

    SECTION("Successful request to delete a channel")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage,
            MessageType::ChannelDeleteRequest, MessageBody::ValidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::Success);
        testKernel->stop();
    }
}

TEST_CASE("Channel request procedures", "[Kernel][Failed]")
{
    auto testKernel = makeTestKernel();
    testKernel->start();

    TestClient client;
    client.connectToKernel(TestKernelInfo::Address::local, TestKernelInfo::Port::test);

    SECTION("Failed request to create a channel")
    {
        Message     invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage, 
            MessageType::ChannelCreateRequest, MessageBody::InvalidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::InvalidBody);
        testKernel->stop();
    }

    SECTION("Failed request subscription to channel")
    {
        Message     invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage, 
            MessageType::ChannelSubscribeRequest, MessageBody::InvalidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
            Client::TestObject::MessageResult::InvalidBody);
        testKernel->stop();
    }
    
    SECTION("Failed request to leave from channel")
    {
        Message     invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage, 
            MessageType::ChannelLeaveRequest, MessageBody::InvalidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::InvalidBody);
        testKernel->stop();
    }

    SECTION("Failed request to delete a channel")
    {
        Message     invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage, 
            MessageType::ChannelDeleteRequest, MessageBody::InvalidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::InvalidBody);
        testKernel->stop();
    }
}
