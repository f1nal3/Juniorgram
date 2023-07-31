#include <memory>

#include "Network/Connection.hpp"
#include "Network/Message.hpp"
#include "Utility/SafeQueue.hpp"

#define APPLY_API_METHODS \
            std::optional<Network::MessageResult> messageClient                  (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            std::optional<Network::MessageResult> messageAllClients              (std::shared_ptr<Network::Connection> exceptionConnection,const Message& message) const override;   \
            std::optional<Network::MessageResult> checkServicePing                (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            std::optional<Network::MessageResult> readAllMessage                 (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            std::optional<Network::MessageResult> messageHistoryRequest          (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            std::optional<Network::MessageResult> messageStoreRequest            (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            std::optional<Network::MessageResult> replyHistoryRequest            (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            std::optional<Network::MessageResult> replyStoreRequest              (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            std::optional<Network::MessageResult> messageDeleteRequest           (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            std::optional<Network::MessageResult> messageEditRequest             (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            std::optional<Network::MessageResult> messageReactionRequest         (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            std::optional<Network::MessageResult> registrationRequest            (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            std::optional<Network::MessageResult> loginRequest                   (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            std::optional<Network::MessageResult> channelLeaveRequest            (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            std::optional<Network::MessageResult> channelSubscribeRequest        (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            std::optional<Network::MessageResult> channelDeleteRequest           (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            std::optional<Network::MessageResult> channelCreateRequest           (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            std::optional<Network::MessageResult> directMessageCreateRequest     (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            std::optional<Network::MessageResult> channelListRequest             (std::shared_ptr<Network::Connection> clientConnection) const override;                           \
            std::optional<Network::MessageResult> channelSubscriptionListRequest (std::shared_ptr<Network::Connection> clientConnection) const override;                           \
            std::optional<Network::MessageResult> defaultRequest() const override;

namespace Network
{
    class iAPI
    {
        public:
        /**
         * @brief Method for sending the message from client.
         * @param Connection management class as std::shared_ptr<Network::Connection>& and Network::Message& class.
         */
        virtual std::optional<MessageResult> messageClient(std::shared_ptr<Connection> clientConnection, const Message& message) const = 0;

        /**
         * @brief Method of sending a message to all clients.
         * @param Network::Message& class and connection management class as std::shared_ptr<Network::Connection>&.
         */
        virtual std::optional<MessageResult> messageAllClients(std::shared_ptr<Connection> exceptionConnection, const Message& message) const = 0;

        /**
         * @brief The method for checking of service ping.
         * @param const std::shared_ptr<Connection>& client - sender context.
         * @param Message& message - for use the message header functionality.
         */
        virtual std::optional<MessageResult> checkServicePing(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking of receiving all messages by the client.
         * @param const std::shared_ptr<Connection>& client - sender context.
         * @param Message& message - for use the message header functionality.
         */
        virtual std::optional<MessageResult> readAllMessage(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method of checking for a channel list.
         * @param const std::shared_ptr<Connection>& client - sender context.
         */
        virtual std::optional<MessageResult> channelListRequest(std::shared_ptr<Connection> client) const = 0;

        /**
         * @brief The method for checking message history extraction.
         * @param const std::shared_ptr<Connection>& client - sender context.
         * @param Message& message - for use the message header functionality.
         */
        virtual std::optional<MessageResult> messageHistoryRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking the retrieval of stored messages.
         * @param const std::shared_ptr<Connection>& client - sender context.
         * @param Message& message - for use the message header functionality.
         */
        virtual std::optional<MessageResult> messageStoreRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking answer history extraction.
         * @param const std::shared_ptr<Connection>& client - sender context.
         * @param Message& message - for use the message header functionality.
         */
        virtual std::optional<MessageResult> replyHistoryRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking repeatedly the retrieval of stored messages.
         * @param const std::shared_ptr<Connection>& client - sender context.
         * @param Message& message - for use the message header functionality.
         */
        virtual std::optional<MessageResult> replyStoreRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking of deletion a message request.
         * @param const std::shared_ptr<Connection>& client - sender context.
         * @param Message& message - for use the message header functionality.
         */
        virtual std::optional<MessageResult> messageDeleteRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking the editing of a query.
         * @param const std::shared_ptr<Connection>& client - sender context.
         * @param Message& message - for use the message header functionality.
         */
        virtual std::optional<MessageResult> messageEditRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking the reaction.
         * @param const std::shared_ptr<Connection>& client - sender context.
         * @param Message& message - for use the message header functionality.
         */
        virtual std::optional<MessageResult> messageReactionRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking the registration request.
         * @param const std::shared_ptr<Connection>& client - sender context.
         * @param Message& message - for use the message header functionality.
         */
        virtual std::optional<MessageResult> registrationRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking the logging request.
         * @param const std::shared_ptr<Connection>& client - sender context.
         * @param Message& message - for use the message header functionality.
         */
        virtual std::optional<MessageResult> loginRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking the exit from the channel.
         * @param const std::shared_ptr<Connection>& client - sender context.
         * @param Message& message - for use the message header functionality.
         */
        virtual std::optional<MessageResult> channelLeaveRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for verifying a channel subscription.
         * @param const std::shared_ptr<Connection>& client - sender context.
         * @param Message& message - for use the message header functionality.
         */
        virtual std::optional<MessageResult> channelSubscribeRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking the verification of users subscribed to a channel.
         * @param const std::shared_ptr<Connection>& client - sender context.
         */
        virtual std::optional<MessageResult> channelSubscriptionListRequest(std::shared_ptr<Connection> client) const = 0;

        /**
         * @brief The method of checking the deletion of a channel.
         * @param const std::shared_ptr<Connection>& client - sender context.
         * @param Message& message - for use the message header functionality.
         */
        virtual std::optional<MessageResult> channelDeleteRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method of checking the creation of a channel.
         * @param const std::shared_ptr<Connection>& client - sender context.
         * @param Message& message - for use the message header functionality.
         */
        virtual std::optional<MessageResult> channelCreateRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking direct messages received by the user.
         * @param const std::shared_ptr<Connection>& client - sender context.
         * @param Message& message - for use the message header functionality.
         */
        virtual std::optional<MessageResult> directMessageCreateRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for getting default request.
         */
        virtual std::optional<MessageResult> defaultRequest() const = 0;

        virtual ~iAPI() = default;
    };
} /// namespace Network
