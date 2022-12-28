#include <memory>

#include "Network/Connection.hpp"
#include "Network/Message.hpp"
#include "Network/SafeQueue.hpp"

using ClientConn = const std::shared_ptr<Network::Connection>&;

#define APPLY_API_METHODS ;\
            void messageClient                  (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            void messageAllClients              (std::shared_ptr<Network::Connection> exceptionConnection,const Message& message) const override;   \
            void checkServerPing                (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            void readAllMessage                 (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            void channelListRequest             (std::shared_ptr<Network::Connection> clientConnection) const override;                             \
            void messageHistoryRequest          (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            void messageStoreRequest            (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            void replyHistoryRequest            (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            void replyStoreRequest              (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            void messageDeleteRequest           (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            void messageEditRequest             (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            void messageReactionRequest         (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            void registrationRequest            (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            void loginRequest                   (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            void channelLeaveRequest            (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            void channelSubscribeRequest        (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            void channelSubscriptionListRequest (std::shared_ptr<Network::Connection> clientConnection) const override;                             \
            void channelDeleteRequest           (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            void channelCreateRequest           (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            void directMessageCreateRequest     (std::shared_ptr<Network::Connection> clientConnection,   const Message& message) const override;   \
            void defaultRequest                 () const override;





namespace Network{
    class iAPI
    {
        public:

        /**
         * @brief Method for sending the message from client.
         * @param Connection management class as std::shared_ptr<Network::Connection>& and Network::Message& class.
         */
        virtual void messageClient(std::shared_ptr<Connection> clientConnection, const Message& message) const = 0;

        /**
         * @brief Method of sending a message to all clients.
         * @param Network::Message& class and connection management class as std::shared_ptr<Network::Connection>&.
         */
        virtual void messageAllClients(std::shared_ptr<Connection> exceptionConnection, const Message& message) const = 0;

        /**
         * @brief The method for checking of server ping.
         * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
         * @param Message& message - for use the message header functionality.
         */
        virtual void checkServerPing(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking of receiving all messages by the client.
         * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
         * @param Message& message - for use the message header functionality.
         */
        virtual void readAllMessage(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method of checking for a channel list.
         * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
         */
        virtual void channelListRequest(std::shared_ptr<Connection> client) const = 0;

        /**
         * @brief The method for checking message history extraction.
         * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
         * @param Message& message - for use the message header functionality.
         */
        virtual void messageHistoryRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking the retrieval of stored messages.
         * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
         * @param Message& message - for use the message header functionality.
         */
        virtual void messageStoreRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking answer history extraction.
         * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
         * @param Message& message - for use the message header functionality.
         */
        virtual void replyHistoryRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking repeatedly the retrieval of stored messages.
         * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
         * @param Message& message - for use the message header functionality.
         */
        virtual void replyStoreRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking of deletion a message request.
         * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
         * @param Message& message - for use the message header functionality.
         */
        virtual void messageDeleteRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking the editing of a query.
         * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
         * @param Message& message - for use the message header functionality.
         */
        virtual void messageEditRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking the reaction.
         * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
         * @param Message& message - for use the message header functionality.
         */
        virtual void messageReactionRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking the registration request.
         * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
         * @param Message& message - for use the message header functionality.
         */
        virtual void registrationRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking the logging request.
         * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
         * @param Message& message - for use the message header functionality.
         */
        virtual void loginRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking the exit from the channel.
         * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
         * @param Message& message - for use the message header functionality.
         */
        virtual void channelLeaveRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for verifying a channel subscription.
         * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
         * @param Message& message - for use the message header functionality.
         */
        virtual void channelSubscribeRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking the verification of users subscribed to a channel.
         * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
         */
        virtual void channelSubscriptionListRequest(std::shared_ptr<Connection> client) const = 0;

        /**
         * @brief The method of checking the deletion of a channel.
         * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
         * @param Message& message - for use the message header functionality.
         */
        virtual void channelDeleteRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method of checking the creation of a channel.
         * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
         * @param Message& message - for use the message header functionality.
         */
        virtual void channelCreateRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for checking direct messages received by the user.
         * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
         * @param Message& message - for use the message header functionality.
         */
        virtual void directMessageCreateRequest(std::shared_ptr<Connection> client, const Message& message) const = 0;

        /**
         * @brief The method for getting default request.
         */
        virtual void defaultRequest() const = 0;

        virtual iAPI::~iAPI() = default;
        };
}
