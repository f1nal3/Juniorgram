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
*   It can request certain data from the server. \
*   The data received from the server is provided to the user.
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
    * @brief Connection check.
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
    * @details Every type of message have specific function handler (see here...). \
    *   A list of all message types can be found here.
    */ 
    void loop();

    /**
    * @brief Ping a server.
    * @details This function handler of specific message type. \
    *   Sends a request to check \
    *   the status of the connection to the server.
    */ 
    void pingServer() const;

    /**
    * @brief Create direct chat.
    * @details This function handler of specific message type.
    */ 
    void createDirectChat(uint64_t receiverId) const;

    /**
    * @brief Ask for channel list.
    * @details This function handler of specific message type.
    */ 
    void askForChannelList() const;

    /**
    * @brief Ask for channel subscription list.
    * @details This function handler of specific message type.
    */ 
    void askForChannelSubscriptionList() const;

    /**
    * @brief Ask for channel message history.
    * @details This function handler of specific message type.
    */
    void askForMessageHistory(uint64_t channelID) const;

    /**
    * @brief Ask for reply message history.
    * @details This function handler of specific message type.
    */
    void askForReplyHistory(uint64_t channelID) const;

    /**
    * @brief Sends a none message to all clients.
    * @details This function handler of specific message type.
    */
    void messageAll() const;

    /**
    * @brief Delete user's message.
    * @details This function handler of specific message type.
    */
    void userMessageDelete(const std::uint64_t messageID) const;

    /**
    * @brief Delete user's message.
    * @details This function handler of specific message type.
    */
    void userMessageDelete(const std::string& messageText) const;

    /**
    * @brief Sending the ID of the subscribed channel.
    * @details This function handler of specific message type.
    */
    void subscriptionChannel(const std::uint64_t channelID) const;

    /**
    * @brief Sending the name of the leave channel.
    * @details This function handler of specific message type.
    */
    void leaveChannel(const std::string_view& channelName) const;

    /**
    * @brief Sending the name of the delete channel.
    * @details This function handler of specific message type.
    */
    void deleteChannel(const std::string_view& channelName) const;

    /**
    * @brief Sending the info of the created channel.
    * @details This function handler of specific message type.
    */
    void createChannel(const std::string_view& channelName) const;

    /**
    * @brief Send a message to server.
    * @details This function handler of specific message type.
    */
    void storeMessage(const std::string& message, uint64_t channelID) const;

    /**
    * @brief Send a reply to server.
    * @details This function handler of specific message type.
    */
    void storeReply(const std::string& message, uint64_t channelID, uint64_t msgID) const;

    /**
    * @brief Send an registration request to server.
    * @details This function handler of specific message type.
    *   Generating password's hash which are based on login. \
    *   It lets us to insert different users with the same passwords.
    */
    void userRegistration(const std::string& email, const std::string& login, const std::string& password) const;

    /**
    * @brief Send an authorization request to server.
    * @details This function handler of specific message type.
    */
    void userAuthorization(const std::string& login, const std::string& password) const;

    /**
    * @brief Send user reaction change to database.
    * @details This function handler of specific message type.
    * @param Reaction ID as std::uint32_t reactionID.
    *   Using max uint32_t as special value.
    *   Supported reaction IDs are 0(like), 1(dislike), 2(fire), 3(cat), 4(smile).
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
    bool _serverAccept = false;
    asio::io_context _context;
    std::thread      _contextThread;
    std::unique_ptr<Connection> _connection;
    Utility::SafeQueue<Message> _incomingMessagesQueue;
};
}  // namespace Network
