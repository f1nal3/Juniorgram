#pragma once

#include "Network/Connection.hpp"
#include <Models/Primitives.hpp>
#include <FileLogger.hpp>

#include "TestServerInfo.hpp"

namespace TestObject
{
using Network::Message;
using Network::Connection;

/**
* @brief TestClient class.
*/
class TestClient
{
public:
    virtual ~TestClient() noexcept;

    bool connectToServer(const std::string_view& host, const uint16_t port);
    void disconnectFromServer();

    void send(const Message& message) const;

private:
    void               loop();
    [[nodiscard]] bool isConnected() const;
    bool               checkServerAcception();
    bool               checkConnectionArguments(const std::string_view& host, const uint16_t port);

    bool                                 _serverAccept = false;
    asio::io_context                     _context;
    std::thread                          _contextThread;
    std::unique_ptr<Network::Connection> _connection;
    Utility::SafeQueue<Message>          _incomingMessagesQueue;

protected:
    /**
    * @brief Method for signaling of disconnect from server.
    */
    void onDisconnect();

    /**
    * @brief Method for signaling of sending message to server.
    */
    void onMessageSendFailed(const Message& message) const;

    /**
    * @brief Method for signaling of login answer.
    */
    void onLoginAnswer(bool success);

    /**
    * @brief Method for signaling of server accepting.
    */
    void onServerAccepted();

    /**
    * @brief Method for signaling of server ping.
    */
    void onServerPing(double timestamp);

    /**
    * @brief Method for signaling of server message.
    */
    void onServerMessage(const uint64_t clientID);

    /**
    * @brief Method for signaling of channel list request.
    */
    void onChannelListRequest(const std::vector<Models::ChannelInfo>& channels);

    /**
    * @brief Method for signaling of message history answer.
    */
    void onMessageHistoryAnswer(const std::vector<Models::MessageInfo>& messages);

    /**
    * @brief Method for signaling of message store answer.
    */
    void onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode);

    /**
    * @brief Method for signaling of message delete answer.
    */
    void onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState);

    /**
    * @brief Method for signaling of registration answer.
    */
    void onRegistrationAnswer(Utility::RegistrationCodes registrationCode);

    /**
    * @brief Method for signaling of reply history answer.
    */
    void onReplyHistoryAnswer(const std::vector<Models::ReplyInfo>& replies);

    /**
    * @brief Method for signaling of reply store answer.
    */
    void onReplyStoreAnswer(Utility::StoringReplyCodes storingReplyCode);

    /**
    * @brief Method for signaling of channel leave answer.
    */
    void onChannelLeaveAnswer(Utility::ChannelLeaveCodes channelLeaveCode);

    /**
    * @brief Method for signaling of channel subscription answer.
    */
    void onChannelSubscribingAnswer(const Utility::ChannelSubscribingCodes subscribingChannelCode);

    /**
    * @brief Method for signaling of channel subscription list answer.
    */
    void onChannelSubscribingListAnswer(const std::vector<uint64_t>& subscribingChannelList);

    /**
    * @brief Method for signaling of channel delete answer.
    */
    void onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode);

    /**
    * @brief Method for signaling of channel create answer.
    */
    void onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode);

    /**
    * @brief Method for signaling of direct message create answer.
    */
    void onDirectMessageCreateAnswer(Utility::DirectMessageStatus directMessageCreateAnswer);

    /**
    * @brief Method for signaling of reaction answer.
    */
    void onMessageReactionAnswer(Utility::ReactionMessageCodes reactionState);

    /**
    * @brief Method for signaling of edit message answer.
    */
    void onEditMessageAnswer(Utility::EditingMessageCodes reactionState) const;
};
}  /// namespace TestObject
