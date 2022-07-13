#pragma once

#include <iostream>
#include <string>

#include <asio.hpp>
#include <future>

#include <Utility/UtilityTime.hpp>
#include <Network/Connection.hpp>
#include <Network/Primitives.hpp>
#include <Client.Core/Client.Core.Static/ServerInfo.hpp>

#include <MockPostgreRepository.hpp>

namespace MockClient
{
using asio::io_context;
using asio::ip::tcp;
using Network::Connection;
using Network::LoginInfo;
using Network::Message;
using Network::MessageInfo;
using Network::RegistrationInfo;
using Network::ReplyInfo;
using Network::SafeQueue;
using RTC = std::chrono::system_clock;
using UtilityTime::timestamp_t;

/**
 *  @class MockClient.
 *  @brief This class is designed to emulate a client connection.
 *  @details Uses std::asio tools.
 */
class MockClient
{
public:
    bool connectToServer(const std::string_view& host, const uint16_t port);

    void disconnectFromServer();

    bool isConnected() const;

    void send(const Message& message) const;

    void loop();

    SafeQueue<Message>& getMessage();

protected:
    void onDisconnect() const;

    void onMessageSendFailed() const;

    /// Login Answer handler
    void onLoginAnswer(bool success) const;

    /// Server Accepted handler
    void onServerAccepted() const;

    /// Server Ping handler
    void onServerPing(double timestamp) const;

    /// Server Message handler
    void onServerMessage(const uint64_t clientID) const;

    /// Channel List Request handler
    void onChannelListRequest(const std::vector<Network::ChannelInfo>& channels) const;

    /// Message History Answer handler
    void onMessageHistoryAnswer(const std::vector<Network::MessageInfo>& messages) const;

    /// Message Store Answer handler
    void onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode) const;

    /// Message Delete Answer handler
    void onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState) const;

    /// Registration Answer handler
    void onRegistrationAnswer(Utility::RegistrationCodes registrationCode) const;

    /// Reply History Answer heandler
    void onReplyHistoryAnswer(Utility::StoringReplyCodes storingReplyCode) const;

    /// Reply Store Answer handler
    void onReplyStoreAnswer(Utility::StoringReplyCodes storingReplyCode) const;

    /// Channel leave Answer handler
    void onChannelLeaveAnswer(Utility::ChannelLeaveCodes channelLeaveCode) const;

    /// Channel subscription Answer handler
    void onChannelSubscribingAnswer(const Utility::ChannelSubscribingCodes subscribingChannelCode) const;

    /// Channel subscription list Answer handler
    void onChannelSubscribingListAnswer(const std::vector<uint64_t> subscribingChannelList) const;

    /// Channel delete Answer handler
    void onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode) const;

    /// Channel create Answer handler
    void onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode) const;

    /// Direct message create Answer handler
    void onDirectMessageCreateAnswer(Utility::DirectMessageStatus directMessageCreateAnswer) const;

    /// Reaction Answer handler
    void onMessageReactionAnswer(Utility::ReactionMessageCodes reactionState) const;

    void onEditMessageAnswer(Utility::EditingMessageCodes reactionState) const;

private:
    io_context                  testContext;
    std::thread                 testContextThread;
    std::unique_ptr<Connection> testConnection;
    SafeQueue<Message>          testIncomingMessagesQueue;

    bool serverAccept = false;
};
}  // namespace MockClient
