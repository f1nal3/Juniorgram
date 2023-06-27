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
* @details The application core, which is a typical client in a client-kernel architecture. \
*    The class manages the connection and provides a communication interface to the kernel side by providing a set of methods (API). \
*    Each such function defines the data and order of actions that are needed to perform some functionality. \
*    However, an option is provided to send messages manually.(@see send) \
*    See the list of API functions and their descriptions in the class methods.
*/
class Client
{
public:
    /**
    * @brief Client virtual destructor.
    * @details The virtual destructor calls the disconnectFromKernel method, \
    *    which checks the connection to the kernel. 
    */
    virtual ~Client() noexcept;

    /**
    * @brief Connect to kernel with IP(host) and Port(port).
    * @param const std::string_view& host - for identifying and accepting ip address, \
    * @param const uint16_t port - for accepting & identifying of port.
    */
    bool connectToKernel(const std::string_view& host, const uint16_t port);

    /**
    * @brief Disconnect from kernel.
    * @details This method stop connection to remote host. \
    *   Also, it reset all context and jobs that related with Client. \
    *   This method call in destructor, so in general way it is not used.
    */ 
    void disconnectFromKernel();

    /**
    * @brief Checking the kernel connection.
    * @details Returns true if connection is established.
    */ 
    [[nodiscard]] bool isConnected() const;

    /**
    * @brief Sends an initialized header message to the kernel.
    * @details You can see the message types in the Message header file.
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
    * @details Handler function to process the kernel response message \
    *   for the subsequent response from the client.
    *   (You can see the message types in the Message header file).
    */ 
    void loop();

    /**
    * @brief Checking the kernel network transmission delay status.
    * @details The central method in which most of the work of the client kernel takes place.
    *   The function is an infinite loop that waits for messages to appear in the message queue.
    *   Messages are divided into types and a different handler is called for each type.
    *   In the case of a non-registered type, an issue is logged and the work continues.
    */
    void pingKernel() const;

    /**
    * @brief Sends a request to create direct chat.
    * @details receiverID is necessary for proper chat creation.
    */ 
    void createDirectChat(uint64_t receiverId) const;

    /**
    * @brief Sends a request to ask for channel list. 
    */ 
    void askForChannelList() const;

    /**
    * @brief Sends a request to ask for channel subscription list.
    */ 
    void askForChannelSubscriptionList() const;

    /**
    * @brief Sends a request to ask for message history.
    * @param channelID for the correct answer to getting the story.
    */
    void askForMessageHistory(uint64_t channelID) const;

    /**
    * @brief Sends a request to ask for reply history.
    * @param channelID is needed to get a response \
    *   from a particular channel.
    */
    void askForReplyHistory(uint64_t channelID) const;

    /**
    * @brief Sends a request for output all messages.
    */
    void messageAll() const;

    /**
    * @brief Delete user's message.
    * @param messageID is his number at which the message will be deleted.
    */
    void userMessageDelete(const std::uint64_t messageID) const;

    /**
    * @brief Delete user's message.
    * @param messageText is message that will be deleted.
    * @details Overloading a function with the messageID parameter.
    */
    void userMessageDelete(const std::string& messageText) const;

    /**
    * @brief Sends a request to subscription channel.
    * @param ChannelID for the correct action of the subscription.
    */
    void subscriptionChannel(const std::uint64_t channelID) const;

    /**
    * @brief Sends a request to leave channel.
    * @param channelName is necessary to correctly exit 
    *   the channel with the correct name.
    */
    void leaveChannel(const std::string_view& channelName) const;

    /**
    * @brief Sends a request to delete channel.   
    * @param channelName is necessary to correctly delete 
    *   a channel with the desired name.
    */
    void deleteChannel(const std::string_view& channelName) const;

    /**
    * @brief Sends a request to create channel.
    * @param channelName is necessary to correctly create 
    *   a channel with the desired name.
    */
    void createChannel(const std::string_view& channelName) const;

    /**
    * @brief Sends a request to store message.
    * @param channelID and it's message are necessary \
    *   to correctly request message storage. 
    */
    void storeMessage(const std::string& message, uint64_t channelID) const;

    /**
    * @brief Sends a request to store reply.
    * @param channelID, message and its msgID are necessary needed \
    *   to properly request a response from the database \
    *   to retrieve a stored message. 
    */
    void storeReply(const std::string& message, uint64_t channelID, uint64_t msgID) const;

    /**
    * @brief Sends a request to registration of user.
    * @param login and password are needed to hash the password to a certain user. \
    *   Email is necessary to assign a user to it and to restore the account \
    *   in case the password is lost.
    */
    void userRegistration(const std::string& email, const std::string& login, const std::string& password) const;

    /**
    * @brief Sends a request to authorization of user.
    * @param login and password are needed to hash the password to a certain user.
    * @details Allows us to enter different users with the same password. 
    */
    void userAuthorization(const std::string& login, const std::string& password) const;

    /**
    * @brief A method that sends a query to the database to change the reaction state.
    * @details Supported reaction IDs are 0(likes), 1(dislikes), 2(fires), 3(cats), 4(smiles). \
    *   (You can see a list of reactions in the header file Primitives).
    */
    void userMessageReaction(const std::uint64_t messageID, const std::uint32_t reactionID) const;

protected:
    /**
    * @brief Disconnect handler.
    * @details This function handler outputs the response \
    *   to the disconnect from the kernel.
    */
    virtual void onDisconnect();

    /**
    * @brief Message send failure handler.
    * @details This function handler outputs the response \
    *   to an incorrect message being sent.
    */
    virtual void onMessageSendFailed(const Message& message) const;

    /**
    * @brief Login Answer handler.
    * @details This function handler outputs the authorization response.
    */
    virtual void onLoginAnswer(bool success);

    /**
    * @brief Kernel Accepted handler.
    * @details This function handler outputs the response \
    *   on accepting the connection to the kernel.
    */
    virtual void onKernelAccepted();

    /**
    * @brief Kernel Ping handler.
    * @details This function handler outputs the ping status response.
    */
    virtual void onKernelPing(double timestamp);

    /**
    * @brief Kernel Message handler.
    * @details This function handler outputs the message response from the kernel.
    */
    virtual void onKernelMessage(const uint64_t clientId);

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
    * @brief Reply History Answer handler.
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
    * @details This function handler outputs message of channel \
    *   subscribing list answer response.
    */
    virtual void onChannelSubscribingListAnswer(const std::vector<uint64_t>& subscribingChannelList);

    /**
    * @brief Channel delete Answer handler.
    * @details This function handler outputs message of delete answer response.
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
    bool _kernelAccept = false;

    asio::io_context _context;
    std::thread      _contextThread;
    std::unique_ptr<Connection> _connection;
    Utility::SafeQueue<Message> _incomingMessagesQueue;
};
}  // namespace Network
