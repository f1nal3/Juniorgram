#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("TestServerGivingAllMessagesRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message validMessage;
    auto    messageInstance = makeMessage(validMessage, 
        MessageType::MessageAll, MessageBody::ValidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerHistoryRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message validMessage;
    auto    messageInstance = makeMessage(validMessage, 
        MessageType::MessageHistoryRequest, MessageBody::ValidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    auto channelInfo = std::any_cast<uint64_t>(validMessage.mBody);
    REQUIRE(channelInfo == testChannelID);

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerReplyingHistoryRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message validMessage;
    auto    messageInstance = makeMessage(validMessage, 
        MessageType::ReplyHistoryRequest, MessageBody::ValidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    auto channelInfo = std::any_cast<uint64_t>(validMessage.mBody);
    REQUIRE(channelInfo == testChannelID);

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerCreatingDirectMessage [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message validMessage;
    auto    messageInstance = makeMessage(validMessage, 
        MessageType::DirectMessageCreateRequest, MessageBody::ValidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    auto channelInfo = std::any_cast<uint64_t>(validMessage.mBody);
    REQUIRE(channelInfo == testRecipientID);

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerMessageStoreRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message validMessage;
    auto    messageInstance = makeMessage(validMessage, 
        MessageType::MessageStoreRequest, MessageBody::ValidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    auto channelInfo = std::any_cast<MessageInfo>(validMessage.mBody);
    REQUIRE(channelInfo._channelID == testChannelID);

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerReplyingStoreRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message validMessage;
    auto    messageInstance = makeMessage(validMessage, 
        MessageType::ReplyStoreRequest, MessageBody::ValidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    auto channelInfo = std::any_cast<ReplyInfo>(validMessage.mBody);
    REQUIRE(channelInfo._channelID == testChannelID);

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerReactionRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message validMessage;
    auto    messageInstance = makeMessage(validMessage, 
        MessageType::MessageReactionRequest, MessageBody::ValidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    auto channelInfo = std::any_cast<MessageInfo>(validMessage.mBody);
    REQUIRE(channelInfo._msgID == testMsgID);

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerMessageEditingRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message validMessage;
    auto    messageInstance = makeMessage(validMessage, 
        MessageType::MessageEditRequest, MessageBody::ValidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    auto channelInfo = std::any_cast<MessageInfo>(validMessage.mBody);
    REQUIRE(channelInfo._msgID == testMsgID);

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerMessageDeletingRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message validMessage;
    auto    messageInstance = makeMessage(validMessage, 
        MessageType::MessageDeleteRequest, MessageBody::ValidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    auto channelInfo = std::any_cast<MessageInfo>(validMessage.mBody);
    REQUIRE(channelInfo._msgID == testMsgID);

    testServer->update();
    testServer->stop();
}
