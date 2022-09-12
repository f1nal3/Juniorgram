#pragma once

#include "Network/Connection.hpp"
#include "Network/Message.hpp"
#include "Utility/SafeQueue.hpp"

#include <Models/Primitives.hpp>

#include <FileLogger.hpp>

namespace Network
{
/**
* @class Client
* @brief Class for handling connection
*/
class Client
{
public:
    /**
    * @brief Client virtual destructor.
    */
    virtual ~Client();

    /**
    * @brief Connect to server with IP(host) and Port(port).
    * @param const std::string_view& host - for identifing and accepting ip address,\
    *        const uint16_t port - for accepting & identifing of port.
    */
    bool connectToServer(const std::string_view& host, const uint16_t port);

    /**
    * @brief Disconnect from server.
    */ 
    void disconnectFromServer();

    /**
    * @brief Returns true if connection is established.
    */ 
    [[nodiscard]] bool isConnected() const;

    /**
    * @brief Sends a message to server.
    * @param const Message& message - initialization accepting message.
    */ 
    void send(const Message& message) const;

    /**
    * @brief Incoming message queue.
    * @return Utility::SafeQueue<Message> - incoming queue.
    */ 
    Utility::SafeQueue<Message>& incoming() { return _incomingMessagesQueue; }

    /**
    * @brief Loop handler.
    *   Checking the queue for the elements in it.
    */ 
    void loop();

    /**
    * @brief Ping a server.
    */ 
    void pingServer() const;

    /**
    * @brief Create direct chat.
    */ 
    void createDirectChat(uint64_t receiverId) const;

    /**
    * @brief Ask for channel list.
    */ 
    void askForChannelList() const;

    /**
    * @brief Ask for channel subscription list.
    */ 
    void askForChannelSubscriptionList() const;

    /**
    * @brief Ask for channel message history
    * @param channelID Channel ID
    */
    void askForMessageHistory(uint64_t channelID) const;

    /**
    * @brief Ask for reply message history
    * @param channelID
    */
    void askForReplyHistory(uint64_t channelID) const;

    /**
    * @brief Sends a none message to all clients
    */
    void messageAll() const;

    /**
    * @brief Delete user's message
    * @param messageId message ID
    */
    void userMessageDelete(const std::uint64_t messageID) const;

    /**
    * @brief Delete user's message
    * @param messageId message's text
    */
    void userMessageDelete(const std::string& messageText) const;

    /**
    * @brief Sending the ID of the subscribed channel
    * @param message ID uint64_t variable
    */
    void subscriptionChannel(const std::uint64_t channelID) const;

    /**
    * @brief Sending the name of the leave channel
    * @param channel name std::string variable
    */
    void leaveChannel(const std::string channelName) const;

    /**
    * @brief Sending the name of the delete channel
    * @param channel name std::string variable
    */
    void deleteChannel(const std::string channelName) const;

    /**
    * @brief Sending the info of the created channel
    * @param channel name std::string variable
    */
    void createChannel(const std::string channelName) const;

    /**
    * @brief Send a message to server
    * @param message Message
    * @param channelID Channel ID
    */
    void storeMessage(const std::string& message, uint64_t channelID) const;

    /**
    * @brief Send a reply to server
    * @param string Message
    * @param uint64_t channelID
    * @param uint64_t msgID
    */
    void storeReply(const std::string& message, uint64_t channelID, uint64_t msgID) const;

    /**
    * @brief Send an registration request to server
    * @param email User E-Mail
    * @param login User Login
    * @param password Password hashed
    */
    void userRegistration(const std::string& email, const std::string& login, const std::string& password) const;

    /**
    * @brief Send an authorization request to server
    * @param login User login
    * @param password User password
    */
    void userAuthorization(const std::string& login, const std::string& password) const;

    /**
    * @brief Send user reaction change to database
    * @param Message ID as std::uint64_t messageID
    * @param Reaction ID as std::uint32_t reactionID
    * @details Supported reaction IDs are 0(like), 1(dislike), 2(fire), 3(cat), 4(smile)
    */
    void userMessageReaction(const std::uint64_t messageID, const std::uint32_t reactionID) const;

protected:
    /**
    * @brief Disconnect handler.
    */
    virtual void onDisconnect();

    /**
    * @brief Message send failure handler.
    */
    virtual void onMessageSendFailed(const Message& message) const;

    /**
    * @brief Login Answer handler.
    */
    virtual void onLoginAnswer(bool success);

    /**
    * @brief Server Accepted handler.
    */
    virtual void onServerAccepted();

    /**
    * @brief Server Ping handler.
    */
    virtual void onServerPing(double timestamp);

    /**
    * @brief Server Message handler.
    */
    virtual void onServerMessage(const uint64_t clientId);

    /**
    * @brief Channel List Request handler.
    */
    virtual void onChannelListRequest(const std::vector<Models::ChannelInfo>& channels);

    /**
    * @brief Message History Answer handler.
    */
    virtual void onMessageHistoryAnswer(const std::vector<Models::MessageInfo>& messages);

    /**
    * @brief Message Store Answer handler.
    */
    virtual void onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode);

    /**
    * @brief Message Delete Answer handler.
    */
    virtual void onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState);

    /**
    * @brief Registration Answer handler.
    */
    virtual void onRegistrationAnswer(Utility::RegistrationCodes registrationCode);

    /**
    * @brief Reply History Answer heandler.
    */
    virtual void onReplyHistoryAnswer(const std::vector<Models::ReplyInfo>& replies);

    /**
    * @brief Reply Store Answer handler.
    */
    virtual void onReplyStoreAnswer(Utility::StoringReplyCodes storingReplyCode);
 
    /**
    * @brief Channel leave Answer handler.
    */
    virtual void onChannelLeaveAnswer(Utility::ChannelLeaveCodes channelLeaveCode);

    /**
    * @brief Channel subscription Answer handler.
    */
    virtual void onChannelSubscribingAnswer(const Utility::ChannelSubscribingCodes subscribingChannelCode);
 
    /**
    * @brief Channel subscription list Answer handler.
    */
    virtual void onChannelSubscribingListAnswer(const std::vector<uint64_t>& subscribingChannelList);

    /**
    * @brief Channel delete Answer handler.
    */
    virtual void onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode);

    /**
    * @brief Channel edit Answer handler.
    */ 
    virtual void onEditMessageAnswer(Utility::EditingMessageCodes reactionState) const;
 
    /**
    * @brief Channel create Answer handler.
    */
    virtual void onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode);

    /**
    * @brief Direct message create Answer handler.
    */
    virtual void onDirectMessageCreateAnswer(Utility::DirectMessageStatus directMessageCreateAnswer);

    /**
    * @brief Reaction Answer handler.
    */
    virtual void onMessageReactionAnswer(Utility::ReactionMessageCodes reactionState);
    
private:
    asio::io_context _context;
    std::thread      _contextThread;
    std::unique_ptr<Connection> _connection;
    Utility::SafeQueue<Message> _incomingMessagesQueue;
    bool _serverAccept = false;
};
}  // namespace Network
