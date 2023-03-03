#pragma once

#include "Network/Connection.hpp"
#include <Models/Primitives.hpp>
#include <FileLogger.hpp>

#include "TestServerInfo.hpp"
#include <optional>

namespace TestObject
{
using Network::Message;
using Network::Connection;

enum class MessageResult
{
    InvalidBody,
    Success
};

/**
* @brief TestClient class.
*/
class TestClient
{
public:
    /**
    * @brief Connect to server with IP(host) and Port(port).
    * @param const std::string_view& host - for identifying and accepting ip address, \
    * @param const uint16_t port - for accepting & identifying of port.
    */
    bool connectToServer(const std::string_view& host, const uint16_t port);
    
    /**
    * @brief Disconnect from server.
    * @details This method stop connection to remote host. \
    *   Also, it reset all context and jobs that related with TestClient. \
    *   This method call in destructor, so in general way it is not used.
    */ 
    void disconnectFromServer();

    /**
    * @brief Sends an initialized header message to the server.
    * @details You can see the message types in the Message header file.
    */ 
    void send(const Message& message) const;

    std::vector<MessageResult> getMessageResult() const;

    void countOfErrorResults() const;

    /**
    * @brief TestClient virtual destructor.
    * @details The virtual destructor calls the disconnectFromServer method, \
    *    which checks the connection to the server. 
    */
    virtual ~TestClient() noexcept;

protected:
    /**
    * @brief Method for signaling of disconnect from server.
    */
    void onDisconnect() const;

    /**
    * @brief Method for signaling of sending message to server.
    */
    void onMessageSendFailed(const Message& message) const;

    /**
    * @brief Method for signaling of login answer.
    */
    std::optional<MessageResult> onLoginAnswer(bool success) const;

    /**
    * @brief Method for signaling of server accepting.
    */
    std::optional<MessageResult> onServerAccepted() const;

    /**
    * @brief Method for signaling of server ping.
    */
    std::optional<MessageResult> onServerPing(double timestamp) const;

    /**
    * @brief Method for signaling of server message.
    */
    std::optional<MessageResult> onServerMessage(const uint64_t clientID) const;

    /**
    * @brief Method for signaling of channel list request.
    */
    std::optional<MessageResult> onChannelListRequest(const std::vector<Models::ChannelInfo>& channels) const;

    /**
    * @brief Method for signaling of message history answer.
    */
    std::optional<MessageResult> onMessageHistoryAnswer(const std::vector<Models::MessageInfo>& messages) const;

    /**
    * @brief Method for signaling of message store answer.
    */
    std::optional<MessageResult> onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode) const;

    /**
    * @brief Method for signaling of message delete answer.
    */
    std::optional<MessageResult> onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState) const;

    /**
    * @brief Method for signaling of registration answer.
    */
    std::optional<MessageResult> onRegistrationAnswer(Utility::RegistrationCodes registrationCode) const;

    /**
    * @brief Method for signaling of reply history answer.
    */
    std::optional<MessageResult> onReplyHistoryAnswer(const std::vector<Models::ReplyInfo>& replies) const;

    /**
    * @brief Method for signaling of reply store answer.
    */
    std::optional<MessageResult> onReplyStoreAnswer(Utility::StoringReplyCodes storingReplyCode) const;

    /**
    * @brief Method for signaling of channel leave answer.
    */
    std::optional<MessageResult> onChannelLeaveAnswer(Utility::ChannelLeaveCodes channelLeaveCode) const;

    /**
    * @brief Method for signaling of channel subscription answer.
    */
    std::optional<MessageResult> onChannelSubscribingAnswer(const Utility::ChannelSubscribingCodes subscribingChannelCode) const;

    /**
    * @brief Method for signaling of channel subscription list answer.
    */
    std::optional<MessageResult> onChannelSubscribingListAnswer(const std::vector<uint64_t>& subscribingChannelList) const;

    /**
    * @brief Method for signaling of channel delete answer.
    */
    std::optional<MessageResult> onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode) const;

    /**
    * @brief Method for signaling of channel create answer.
    */
    std::optional<MessageResult> onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode) const;

    /**
    * @brief Method for signaling of direct message create answer.
    */
    std::optional<MessageResult> onDirectMessageCreateAnswer(Utility::DirectMessageStatus directMessageCreateAnswer) const;

    /**
    * @brief Method for signaling of reaction answer.
    */
    std::optional<MessageResult> onMessageReactionAnswer(Utility::ReactionMessageCodes reactionState) const;

    /**
    * @brief Method for signaling of edit message answer.
    */
    std::optional<MessageResult> onEditMessageAnswer(Utility::EditingMessageCodes reactionState) const;

    std::optional<MessageResult> defaultAnswer() const;

private:
    /**
    * @brief Noose that handle incoming messages.
    * @details Handler function to process the server response message \
    *   for the subsequent response from the client.
    *   (You can see the message types in the Message header file).
    */ 
    void               noose();

    /**
    * @brief Checking the server connection.
    * @details Returns true if connection is established.
    */ 
    [[nodiscard]] bool isConnected() const;

    void MessageResultIsError(std::optional<MessageResult> result);

    /**
    * @brief Method checks the connection to the server. 
    * @defails Until the connection is established, \
    *          no further work will be done.
    */
    bool               checkServerAcception();

    /**
    * @brief Method checks if the received arguments to connect to the server are correct.
    * @param const std::string_view& host - for identifying and accepting ip address, \
    * @param const uint16_t port - for accepting & identifying of port.
    */
    bool               checkConnectionArguments(const std::string_view& host, const uint16_t port) const;

    bool                                 _serverAccept = false;
    inline static uint8_t                _countOfError{0}; 
    asio::io_context                     _context;
    std::unique_ptr<Network::Connection> _connection;
    std::thread                          _contextThread;
    std::vector<MessageResult>           _messageResponce;
    Utility::SafeQueue<Message>          _incomingMessagesQueue;
};
}  /// namespace TestObject
