#pragma once

#include <optional>

#include <FileLogger.hpp>
#include <Models/Primitives.hpp>
#include "Network/Connection.hpp"

#include "TestServerInfo.hpp"

namespace Client::TestObject
{
using Network::Message;
using Network::Connection;

/**
* @brief enum class to identify the result of messages, /
*        after they have been processed on the client side.
* @details The results of the messages may be as follows: 
*          InvalidBody  - poorly formed message. /
*          KernelAccept - signals the result of the connection to the kernel. /
*          Success      - correctly formed message.
*/
enum class MessageResult
{
    InvalidBody,
    KernelAccept,
    Success
};

/**
* @brief TestClient class.
*/
class TestClient
{
public:
    /**
    * @brief Connect to kernel with IP(host) and Port(port).
    * @param const std::string_view& host - for identifying and accepting ip address, /
    * @param const uint16_t port - for accepting & identifying of port.
    */
    bool connectToKernel(const std::string_view& host, const uint16_t port);
    
    /**
    * @brief Disconnect from kernel.
    * @details This method stop connection to remote host. /
    *   Also, it reset all context and jobs that related with TestClient. /
    *   This method call in destructor, so in general way it is not used.
    */ 
    void disconnectFromKernel();

    /**
    * @brief Sends an initialized header message to the kernel.
    * @details You can see the message types in the Message header file.
    */ 
    void send(const Message& message) const;

    /**
    * @brief Method to get a container with the results of message processing.
    * @details The results of the messages may be as follows: /
    *          InvalidBody  - poorly formed message. /
    *          KernelAccept - signals the result of the connection to the kernel. /
    *          Success      - correctly formed message.
    */
    std::vector<MessageResult> getMessageResult() const;

    /**
    * @brief Method of counting bad messages.
    */
    void countOfErrorResults();

    /**
    * @brief TestClient virtual destructor.
    * @details The virtual destructor calls the disconnectFromKernel method, /
    *    which checks the connection to the kernel. 
    */
    virtual ~TestClient() noexcept;

protected:
    /**
    * @brief Method for signaling of disconnect from kernel.
    */
    void onDisconnect() const;

    /**
    * @brief Method for signaling of sending message to kernel.
    */
    void onMessageSendFailed(const Message& message) const;

    /**
    * @brief Method for signaling of login answer.
    */
    std::optional<MessageResult> onLoginAnswer(bool success) const;

    /**
    * @brief Method for signaling of kernel accepting.
    */
    std::optional<MessageResult> onKernelAccepted() const;

    /**
    * @brief Method for signaling of kernel ping.
    */
    std::optional<MessageResult> onKernelPing(double timestamp) const;

    /**
    * @brief Method for signaling of kernel message.
    */
    std::optional<MessageResult> onKernelMessage(const uint64_t clientID) const;

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

    /**
    * @brief Method for signaling of default message answer.
    */
    std::optional<MessageResult> defaultAnswer() const;

private:
    /**
    * @brief Noose that handle incoming messages.
    * @details Handler function to process the kernel response message /
    *   for the subsequent response from the client.
    *   (You can see the message types in the Message header file).
    */ 
    void noose();

    /**
    * @brief Checking the kernel connection.
    * @details Returns true if connection is established.
    */ 
    [[nodiscard]] bool isConnected() const;

    /**
    * @brief Method for checking a message for a negative result.
    * @details The container stores both negative and positive results of processing, /
    *          in order to identify problems of message formation or, /
    *          conversely, to verify the correct formation of messages.
    * @param std::optional<MessageResult> result - result of message processing.
    */
    void MessageResultIsError(std::optional<MessageResult> result);

    /**
    * @brief Method checks the connection to the kernel. 
    * @defails Until the connection is established, /
    *          no further work will be done.
    */
    bool checkKernelAcception();

    /**
    * @brief Method checks if the received arguments to connect to the kernel are correct.
    * @param const std::string_view& host - for identifying and accepting ip address, /
    * @param const uint16_t port - for accepting & identifying of port.
    */
    bool checkConnectionArguments(const std::string_view& host, const uint16_t port) const;

    bool                                 _kernelAccept = false;
    uint8_t                              _countOfError; 
    asio::io_context                     _context;
    std::unique_ptr<Network::Connection> _connection;
    std::thread                          _contextThread;
    std::vector<MessageResult>           _messageResponce;
    Utility::SafeQueue<Message>          _incomingMessagesQueue;
};
}  /// namespace  Client::TestObject