#include <catch2/catch.hpp>
#include "TestUtils/TestUtility.hpp"
#include "TestUtils/WaitForTime.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("Message request procedures", "[Kernel][Success]")
{
	auto testKernel = makeTestKernel();
    testKernel->start();

    TestClient client;
    client.connectToKernel(TestKernelInfo::Address::local, TestKernelInfo::Port::test);

    SECTION("Successful message history request")
    {
        Message validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::MessageHistoryRequest, MessageBody::ValidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::Success);
        testKernel->stop();
    }

    SECTION("Successful reply message history request")
    {
        Message validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::ReplyHistoryRequest, MessageBody::ValidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::Success);
        testKernel->stop();
    }

    SECTION("Successful direct message request")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::DirectMessageCreateRequest, MessageBody::ValidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::Success);
        testKernel->stop();
    }

    SECTION("Successful message store request")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::MessageStoreRequest, MessageBody::ValidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::Success);
        testKernel->stop();
    }

    SECTION("Successful reply message store request")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::ReplyStoreRequest, MessageBody::ValidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::Success);
        testKernel->stop();
    }

    SECTION("Successful message reaction request")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::MessageReactionRequest, MessageBody::ValidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::Success);
        testKernel->stop();
    }

    SECTION("Successful message edit request")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::MessageEditRequest, MessageBody::ValidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::Success);
        testKernel->stop();
    }

    SECTION("Successful message delete request")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::MessageDeleteRequest, MessageBody::ValidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::Success);
        testKernel->stop();
    }
}

TEST_CASE("Message request procedures", "[Kernel][Failed]")
{
    auto testKernel = makeTestKernel();
    testKernel->start();

    TestClient client;
    client.connectToKernel(TestKernelInfo::Address::local, TestKernelInfo::Port::test);

    SECTION("Failed message history request")
    {
        Message     invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage,
            MessageType::MessageHistoryRequest, MessageBody::InvalidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::InvalidBody);
        testKernel->stop();
    }

    SECTION("Failed reply message history request")
    {
        Message     invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage,
            MessageType::ReplyHistoryRequest, MessageBody::InvalidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::InvalidBody);
        testKernel->stop();
    }

    SECTION("Failed direct message request")
    {
        Message     invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage,
            MessageType::DirectMessageCreateRequest, MessageBody::InvalidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() ==
             Client::TestObject::MessageResult::InvalidBody);
        testKernel->stop();
    }

    SECTION("Failed message store request")
    {
        Message     invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage,
            MessageType::MessageStoreRequest, MessageBody::InvalidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::InvalidBody);
        testKernel->stop();
    }

    SECTION("Failed reply message store request")
    {
        Message     invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage,
            MessageType::ReplyStoreRequest, MessageBody::InvalidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::InvalidBody);
        testKernel->stop();
    }

    SECTION("Failed message reaction request")
    {
        Message     invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage,
            MessageType::MessageReactionRequest, MessageBody::InvalidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::InvalidBody);
        testKernel->stop();
    }

    SECTION("Failed message edit request")
    {
        Message     invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage,
            MessageType::MessageEditRequest, MessageBody::InvalidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::InvalidBody);
        testKernel->stop();
    }

    SECTION("Failed message delete request")
    {
        Message     invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage,
            MessageType::MessageDeleteRequest, MessageBody::InvalidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() == 
             Client::TestObject::MessageResult::InvalidBody);
        testKernel->stop();
    }
}
