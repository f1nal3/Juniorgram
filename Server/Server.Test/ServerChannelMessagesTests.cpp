#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("TestServerChannelCreatingRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message validMessage;
    auto    messageInstance = makeMessage(validMessage, 
        MessageType::ChannelCreateRequest, MessageBody::ValidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    auto channelInfo = std::any_cast<std::string>(validMessage.mBody);
    REQUIRE(channelInfo == testChannelName);

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerChannelListRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message validMessage;
    auto    messageInstance = makeMessage(validMessage, 
        MessageType::ChannelListRequest, MessageBody::ValidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    auto channelInfo = std::any_cast<std::vector<ChannelInfo>>(validMessage.mBody);
    REQUIRE(channelInfo.data()->_channelID == 0);

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerChannelSubscribingRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message validMessage;
    auto    messageInstance = makeMessage(validMessage, 
        MessageType::ChannelSubscribeRequest, MessageBody::ValidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    auto channelInfo = std::any_cast<ChannelSubscriptionInfo>(validMessage.mBody);
    REQUIRE(channelInfo._channelID == testChannelID);

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerChannelSubscriptionListRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message validMessage;
    auto    messageInstance = makeMessage(validMessage, 
        MessageType::ChannelSubscriptionListRequest, MessageBody::ValidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    auto channelInfo = std::any_cast<ChannelSubscriptionInfo>(validMessage.mBody);
    REQUIRE(channelInfo._channelID == testChannelID);

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerChannelLeavingRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message validMessage;
    auto    messageInstance = makeMessage(validMessage, 
        MessageType::ChannelLeaveRequest, MessageBody::ValidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    auto channelInfo = std::any_cast<std::string>(validMessage.mBody);
    REQUIRE(channelInfo == testChannelName);

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerChannelDeletingRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message validMessage;
    auto    messageInstance = makeMessage(validMessage, 
        MessageType::ChannelDeleteRequest, MessageBody::ValidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    auto channelInfo = std::any_cast<std::string>(validMessage.mBody);
    REQUIRE(channelInfo == testChannelName);

    testServer->update();
    testServer->stop();
}
