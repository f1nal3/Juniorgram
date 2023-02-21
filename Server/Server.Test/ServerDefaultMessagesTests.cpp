#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("TestServerPingRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message validMessage;
    const auto& messageInstance = makeMessage(validMessage,
        MessageType::ServerPing, MessageBody::ValidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    CHECK_NOTHROW(testServer->update());
    REQUIRE_NOTHROW(testServer->stop());
}

TEST_CASE("TestServerFailedDefaultRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message validMessage;
    constexpr auto failedType = int16_t(666);

    const auto& messageInstance = makeMessage(validMessage,
        MessageType(failedType), MessageBody::InvalidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    CHECK_NOTHROW(testServer->update());
    REQUIRE_NOTHROW(testServer->stop());
}
