#include <catch2/catch.hpp>

#include "Utilities/TestUtility.hpp"


using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("Connection request procedures", "[Server][require-false]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);
    Message invalidMessage;

    SECTION("Registration Request")
    {
        const auto& messageInstance = makeMessage(invalidMessage, MessageType::RegistrationRequest, MessageBody::InvalidBody);
        CHECK_NOTHROW(client.send(messageInstance));
    }

    SECTION("LogIn Request")
    {
        const auto& messageInstance = makeMessage(invalidMessage, MessageType::LoginRequest, MessageBody::InvalidBody);
        CHECK_NOTHROW(client.send(messageInstance));
    }

    testServer->update();
    testServer->stop();
}