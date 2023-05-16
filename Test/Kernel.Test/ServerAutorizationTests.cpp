#include <catch2/catch.hpp>
#include "TestUtils/TestUtility.hpp"
#include "TestUtils/WaitForTime.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("Autorization request procedures [Server][Success]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    SECTION("Successful Registration Request")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::RegistrationRequest, MessageBody::ValidBody);

        client.send(messageInstance);
        testServer->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() 
            ==  Client::TestObject::MessageResult::Success);
        testServer->stop();
    }

    SECTION("Successful Logged In Request")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::LoginRequest, MessageBody::ValidBody);

        client.send(messageInstance);
        testServer->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() 
            ==  Client::TestObject::MessageResult::Success);
        testServer->stop();
    }
}

TEST_CASE("Autorization request procedures [Server][Failed]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    SECTION("Failed Registration Request")
    {
        Message invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage, 
        MessageType::RegistrationRequest, MessageBody::InvalidBody);

        client.send(messageInstance);
        testServer->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() 
            ==  Client::TestObject::MessageResult::InvalidBody);
        testServer->stop();
    }

    SECTION("Failed Logged In Request")
    {
        Message invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage,
        MessageType::LoginRequest, MessageBody::InvalidBody);

        client.send(messageInstance);
        testServer->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() 
            ==  Client::TestObject::MessageResult::InvalidBody);
        testServer->stop();
    }
}
