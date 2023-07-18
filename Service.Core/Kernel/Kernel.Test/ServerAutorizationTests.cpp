#include <catch2/catch.hpp>
#include "TestUtils/TestUtility.hpp"
#include "TestUtils/WaitForTime.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("Autorization request procedures", "[Kernel][Success]")
{
    auto testKernel = makeTestKernel();
    testKernel->start();

    TestClient client;
    client.connectToKernel(TestKernelInfo::Address::local, TestKernelInfo::Port::test);

    SECTION("Successful Registration Request")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::RegistrationRequest, MessageBody::ValidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() 
            ==  Client::TestObject::MessageResult::Success);
        testKernel->stop();
    }

    SECTION("Successful Logged In Request")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::LoginRequest, MessageBody::ValidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() 
            ==  Client::TestObject::MessageResult::Success);
        testKernel->stop();
    }
}

TEST_CASE("Autorization request procedures", "[Kernel][Failed]")
{
    auto testKernel = makeTestKernel();
    testKernel->start();

    TestClient client;
    client.connectToKernel(TestKernelInfo::Address::local, TestKernelInfo::Port::test);

    SECTION("Failed Registration Request")
    {
        Message invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage, 
        MessageType::RegistrationRequest, MessageBody::InvalidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() 
            ==  Client::TestObject::MessageResult::InvalidBody);
        testKernel->stop();
    }

    SECTION("Failed Logged In Request")
    {
        Message invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage,
        MessageType::LoginRequest, MessageBody::InvalidBody);

        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() 
            ==  Client::TestObject::MessageResult::InvalidBody);
        testKernel->stop();
    }
}
