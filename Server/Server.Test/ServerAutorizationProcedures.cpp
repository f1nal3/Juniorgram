#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

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

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() 
            == TestObject::MessageResult::Success);
        testServer->stop();
    }

    SECTION("Successful Logged In Request")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::LoginRequest, MessageBody::ValidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() 
            == TestObject::MessageResult::Success);
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

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() 
            == TestObject::MessageResult::InvalidBody);
        testServer->stop();
    }

    SECTION("Failed Logged In Request")
    {
        Message invalidMessage;
        const auto& messageInstance = makeMessage(invalidMessage,
        MessageType::LoginRequest, MessageBody::InvalidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        Sleep(1000);
        REQUIRE(client.getMessageResult().back() 
            == TestObject::MessageResult::InvalidBody);
        testServer->stop();
    }
}