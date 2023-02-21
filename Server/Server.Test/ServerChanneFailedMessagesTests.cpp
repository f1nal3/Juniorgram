#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("TestServerChannelFailedCreatingRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message invalidMessage;
    const auto& messageInstance = makeMessage(invalidMessage, 
        MessageType::ChannelCreateRequest, MessageBody::InvalidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerChannelFailedSubscribingRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message invalidMessage;
    const auto& messageInstance = makeMessage(invalidMessage, 
        MessageType::ChannelSubscribeRequest, MessageBody::InvalidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerChannelFailedLeavingRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message invalidMessage;
    const auto& messageInstance = makeMessage(invalidMessage, 
        MessageType::ChannelLeaveRequest, MessageBody::InvalidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerChannelFailedDeletingRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message invalidMessage;
    const auto& messageInstance = makeMessage(invalidMessage, 
        MessageType::ChannelDeleteRequest, MessageBody::InvalidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    testServer->update();
    testServer->stop();
}
