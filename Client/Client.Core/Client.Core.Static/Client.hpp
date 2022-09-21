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
* @brief Class for handling connection.
* @details Sends requests to the server. \
*   This class contains methods for sending messages to the server \
*   and header methods for processing responses from the server.
*/
class Client
{
public:
    /**
    * @brief Client virtual destructor.
    * @details The virtual destructor calls the disconnectFromServer method, \
    *    which checks the connection to the server. 
    */
    virtual ~Client() noexcept;

    /**
    * @brief Connect to server with IP(host) and Port(port).
    * @param const std::string_view& host - for identifing and accepting ip address, \
    * @param const uint16_t port - for accepting & identifing of port.
    */
    bool connectToServer(const std::string_view& host, const uint16_t port);

    /**
    * @brief Disconnect from server.
    * @details This method stop connection to remote host. \
    *   Also, it reset all context and jobs that related with Client. \
    *   This method call in destructor, so in general way it is not used.
    */ 
    void disconnectFromServer();

    /**
    * @brief Checking the server connection.
    * @details Returns true if connection is established.
    */ 
    [[nodiscard]] bool isConnected() const;

    /**
    * @brief Sends a message to server.
    * @param const Message& message - is sent an initialized header \
    *   message with its own body. 
    */ 
    void send(const Message& message) const;

    /**
    * @brief Incoming message queue.
    * @details Stores the queue of incoming messages that arrive \
    *   to the client after processing and performing a certain action.
    */ 
    Utility::SafeQueue<Message>& incoming();

    /**
    * @brief Loop that handle incoming messages.
    * @details Every type of message have specific function handler, \
    *   which in the process of processing the incoming message from the server goes \
    *   in a loop for further processing of the response from the client.
    */ 
    void loop();

    /**
    * @brief Ping a server.
    * @details This function handler of specific message type. \
    *   Sends a request to check the status of the connection to the server.
    */ 
    void pingServer() const;

    /**
    * @brief Create direct chat.
    * @details This function handler of specific message type. \
    *   Sends a request to create direct chat.
    */ 
    void createDirectChat(uint64_t receiverId) const;

    /**
    * @brief Ask for channel list.
    * @details This function handler of specific message type. \
    *   Sends a request to ask for channel list.
    */ 
    void askForChannelList() const;

    /**
    * @brief Ask for channel subscription list.
    * @details This function handler of specific message type. \
    *   Sends a request to ask for channel subscription list.
    */ 
    void askForChannelSubscriptionList() const;

    /**
    * @brief Ask for channel message history.
    * @details This function handler of specific message type. \
    *   Sends a request to ask for message history.
    */
    void askForMessageHistory(uint64_t channelID) const;

    /**
    * @brief Ask for reply message history.
    * @details This function handler of specific message type. \
    *   Sends a request to ask for reply history.
    */
    void askForReplyHistory(uint64_t channelID) const;

    /**
    * @brief Sends a none message to all clients.
    * @details This function handler of specific message type. \
    *   Sends a request for output all messages. 
    */
    void messageAll() const;

    /**
    * @brief Delete user's message.
    * @details This function handler of specific message type. \
    *   Sends a request to delete message from client side.
    */
    void userMessageDelete(const std::uint64_t messageID) const;

    /**
    * @brief Delete user's message.
    * @details This function handler of specific message type. \
    *   Sends a request to delete message from client side.
    */
    void userMessageDelete(const std::string& messageText) const;

    /**
    * @brief Sending the ID of the subscribed channel.
    * @details This function handler of specific message type. \
    *   Sends a request to subscription channel.
    */
    void subscriptionChannel(const std::uint64_t channelID) const;

    /**
    * @brief Sending the name of the leave channel.
    * @details This function handler of specific message type. \
    *   Sends a request to leave channel.
    */
    void leaveChannel(const std::string_view& channelName) const;

    /**
    * @brief Sending the name of the delete channel.
    * @details This function handler of specific message type. \
    *   Sends a request to delete channel.
    */
    void deleteChannel(const std::string_view& channelName) const;

    /**
    * @brief Sending the info of the created channel.
    * @details This function handler of specific message type. \
    *   Sends a request to create channel.
    */
    void createChannel(const std::string_view& channelName) const;

    /**
    * @brief Send a message to server.
    * @details This function handler of specific message type.\
    *   Sends a request to store message.
    */
    void storeMessage(const std::string& message, uint64_t channelID) const;

    /**
    * @brief Send a reply to server.
    * @details This function handler of specific message type.\
    *   Sends a request to store reply.
    */
    void storeReply(const std::string& message, uint64_t channelID, uint64_t msgID) const;

    /**
    * @brief Send an registration request to server.
    * @details This function handler of specific message type. \
    *   Generating a SHA_256 hash of the password based on the login.\
    *   Sends a request to registration of user.
    */
    void userRegistration(const std::string& email, const std::string& login, const std::string& password) const;

    /**
    * @brief Send an authorization request to server.
    * @details This function handler of specific message type. \
    *   Sends a request to authorization of user. \
    *   Allows us to enter different users with the same password.
    */
    void userAuthorization(const std::string& login, const std::string& password) const;

    /**
    * @brief Send user reaction change to database.
    * @details This function handler of specific message type.
    * @param Reaction ID as std::uint32_t reactionID. \
    *   Using max uint32_t as special value.
    */
    void userMessageReaction(const std::uint64_t messageID, const std::uint32_t reactionID) const;

protected:
    /**
    * @brief Disconnect handler.
    * @details This function handler outputs \
    *   the response to the disconnect from the server.
    */
    virtual void onDisconnect();

    /**
    * @brief Message send failure handler.
    * @details This function handler outputs \
    *   the response to an incorrect message being sent.
    */
    virtual void onMessageSendFailed(const Message& message) const;

    /**
    * @brief Login Answer handler.
    * @details This function handler outputs \
    *   the authorization response.
    */
    virtual void onLoginAnswer(bool success);

    /**
    * @brief Server Accepted handler.
    * @details This function handler outputs the response \
    *   on accepting the connection to the server.
    */
    virtual void onServerAccepted();

    /**
    * @brief Server Ping handler.
    * @details This function handler outputs the ping status response.
    */
    virtual void onServerPing(double timestamp);

    /**
    * @brief Server Message handler.
    * @details This function handler outputs the message response from the server.
    */
    virtual void onServerMessage(const uint64_t clientId);

    /**
    * @brief Channel List Request handler.
    * @details This function handler outputs channel list request response.
    */
    virtual void onChannelListRequest(const std::vector<Models::ChannelInfo>& channels);

    /**
    * @brief Message History Answer handler.
    * @details This function handler outputs message history answer response.
    */
    virtual void onMessageHistoryAnswer(const std::vector<Models::MessageInfo>& messages);

    /**
    * @brief Message Store Answer handler.
    * @details This function handler outputs message store answer response.
    */
    virtual void onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode);

    /**
    * @brief Message Delete Answer handler.
    * @details This function handler outputs message delete answer response.
    */
    virtual void onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState);

    /**
    * @brief Registration Answer handler.
    * @details This function handler outputs registration answer response.
    */
    virtual void onRegistrationAnswer(Utility::RegistrationCodes registrationCode);

    /**
    * @brief Reply History Answer heandler.
    * @details This function handler outputs reply history answer response.
    */
    virtual void onReplyHistoryAnswer(const std::vector<Models::ReplyInfo>& replies);

    /**
    * @brief Reply Store Answer handler.
    * @details This function handler outputs reply store answer response.
    */
    virtual void onReplyStoreAnswer(Utility::StoringReplyCodes storingReplyCode);
 
    /**
    * @brief Channel leave Answer handler.
    * @details This function handler outputs of channel leave answer response.
    */
    virtual void onChannelLeaveAnswer(Utility::ChannelLeaveCodes channelLeaveCode);

    /**
    * @brief Channel subscription Answer handler.
    * @details This function handler outputs of channel subscribing answer response.
    */
    virtual void onChannelSubscribingAnswer(const Utility::ChannelSubscribingCodes subscribingChannelCode);
 
    /**
    * @brief Channel subscription list Answer handler.
    * @details This function handler outputs message of channel subscribing list answer response.
    */
    virtual void onChannelSubscribingListAnswer(const std::vector<uint64_t>& subscribingChannelList);

    /**
    * @brief Channel delete Answer handler.
    * @details This function handler outputs message of detele answer response.
    */
    virtual void onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode);

    /**
    * @brief Channel edit Answer handler.
    * @details This function handler outputs message of edit answer response.
    */ 
    virtual void onEditMessageAnswer(Utility::EditingMessageCodes reactionState) const;
 
    /**
    * @brief Channel create Answer handler.
    * @details This function handler outputs message of channel create answer response.
    */
    virtual void onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode);

    /**
    * @brief Direct message create Answer handler.
    * @details This function handler outputs message of direct answer response.
    */
    virtual void onDirectMessageCreateAnswer(Utility::DirectMessageStatus directMessageCreateAnswer);

    /**
    * @brief Reaction Answer handler.
    * @details This function handler outputs message of reaction answer response.
    */
    virtual void onMessageReactionAnswer(Utility::ReactionMessageCodes reactionState);
    
private:
    bool _serverAccept = false;
    asio::io_context _context;
    std::thread      _contextThread;
    std::unique_ptr<Connection> _connection;
    Utility::SafeQueue<Message> _incomingMessagesQueue;
};
}  // namespace Network
