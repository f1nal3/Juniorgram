#pragma once

#include <chrono>
#include <deque>
#include <iostream>
#include <memory>
#include <thread>

#include <FileLogger.hpp>

#include "Network/Connection.hpp"
#include "Network/Message.hpp"
#include "Utility/SafeQueue.hpp"

namespace DataAccess
{
class PostgreRepositoryManager;
}

namespace Server
{
using asio::ip::tcp;
using Network::Message;
using Utility::SafeQueue;
using Network::Connection;
using Base::Logger::LogLevel;
using Base::Logger::FileLogger;
using UtilityTime::safe_localtime;
using namespace DataAccess;

/**
 *  @class Server class
 *  @brief This class does all logic which is needed to run the server.
 *  @details Uses std::asio tools.
 */
template <typename TManager = PostgreRepositoryManager>
class Server
{
private:
    uint64_t _idCounter         = 10000;
    uint64_t _criticalQueueSize = 100;
    uint64_t _newThreadsCount   = std::thread::hardware_concurrency();

    asio::io_context                                       _context;
    tcp::acceptor                                          _acceptor;
    std::deque<std::shared_ptr<Connection>>                _connectionsPointers;
    SafeQueue<Message>                                     _incomingMessagesQueue;
    std::deque<std::thread>                                _threads;
    std::unique_ptr<TManager>                              _repoManager;

private:
    /**
     * @brief Method for sending the message connecting to the server.
     * @param Connection management class as std::shared_ptr<Network::Connection>&.
     */
    static bool onClientConnect(const std::shared_ptr<Connection>& client)
    {
        Message message;
        message.mHeader.mMessageType = Message::MessageType::ServerAccept;
        client->send(message);
        return true;
    }

    /**
     * @brief Method for sending the message disconnecting from the server.
     * @param Connection management class as std::shared_ptr<Network::Connection>&.
     */
    static void onClientDisconnect(const std::shared_ptr<Connection>& client)
    {
        FileLogger::getInstance().log
        (
            "Removing client [" + std::to_string(client->getID()) + "]", 
            LogLevel::INFO
        );
    }

    /**
     * @brief Method used to process messages.
     * @param Connection management class as std::shared_ptr<Network::Connection>& and Network::Message& class.
     */
    void onMessage(const std::shared_ptr<Connection>& client, Message& message)
    {
        switch (message.mHeader.mMessageType)
        {
            case Message::MessageType::ServerPing:
            {
                checkServerPing(client, message);
                break;
            }

            case Message::MessageType::MessageAll:
            {
                readAllMessage(client, message);
                break;
            }

            case Message::MessageType::ChannelListRequest:
            {
                channelListRequest(client);
                break;
            }

            case Message::MessageType::MessageHistoryRequest:
            {
                messageHistoryRequest(client, message);
                break;
            }

            case Message::MessageType::MessageStoreRequest:
            {
                messageStoreRequest(client, message);
                break;
            }

            case Message::MessageType::ReplyHistoryRequest:
            {
                replyHistoryRequest(client, message);
                break;
            }

            case Message::MessageType::ReplyStoreRequest:
            {
                replyStoreRequest(client, message);
                break;
            }

            case Message::MessageType::MessageDeleteRequest:
            {
                messageDeleteRequest(client, message);
                break;
            }

            case Message::MessageType::MessageEditRequest:
            {
                messageEditRequest(client, message);
                break;
            }

            case Message::MessageType::MessageReactionRequest:
            {
                messageReactionRequest(client, message);
                break;
            }

            case Message::MessageType::RegistrationRequest:
            {
                registrationRequest(client, message);
                break;
            }

            case Message::MessageType::LoginRequest:
            {
                loginRequest(client, message);
                break;
            }

            case Message::MessageType::ChannelLeaveRequest:
            {
                channelLeaveRequest(client, message);
                break;
            }

            case Message::MessageType::ChannelSubscribeRequest:
            {
                channelSubscribeRequest(client, message);
                break;
            }

            case Message::MessageType::ChannelSubscriptionListRequest:
            {
                channelSubscriptionListRequest(client);
                break;
            }

            case Message::MessageType::ChannelDeleteRequest:
            {
                channelDeleteRequest(client, message);
                break;
            }

            case Message::MessageType::ChannelCreateRequest:
            {
                channelCreateRequest(client, message);
                break;
            }

            case Message::MessageType::DirectMessageCreateRequest:
            {
                directMessageCreateRequest(client, message);
                break;
            }

            default:
            {
                defaultRequest();
                break;
            }
        }
    }

public:
    /**
    * @brief Explicit constructor
    * @param const uint16_t& port
    * @details Uses incoming port value from ArgumentParser.
    */
    explicit Server(const uint16_t& port) :
        _acceptor(_context, tcp::endpoint(tcp::v4(), port)),
        _repoManager(std::make_unique<TManager>
            (PostgreAdapter::getInstance<PostgreAdapter>()))
    {
    }
    
    Server(const uint16_t& port, std::unique_ptr<TManager> manager) noexcept :
        _acceptor(_context, tcp::endpoint(tcp::v4(), port)),
        _repoManager(std::move(manager))
    {
    }

    /**
    * @brief Destructor
    */
    ~Server() { stop(); }

    /**
     * @brief Method to start the server.
     */
    void start()
    {
        waitForClientConnection();

        size_t threadsCount = std::thread::hardware_concurrency();
        threadsCount > 1 ? --threadsCount : threadsCount = 1;
    
        for (size_t i = 0; i < threadsCount; ++i)
        {
            _threads.emplace_back(std::thread([this]() { _context.run(); }));
        }
        
        _repoManager->handleRequests();

        FileLogger::getInstance().log
        (
            "[SERVER] Started!", 
            LogLevel::INFO
        );
    }

    //void registerRepositories();

    /**
     * @brief Method to stop the server.
     */
    void stop()
    {
        _context.stop();
        _repoManager->stopHandler();
        // _repoManager->stopHandler();

        for (std::thread& thread : _threads)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }

        _threads.clear();

        FileLogger::getInstance().log
        (
            "[SERVER] Stopped!",
            LogLevel::INFO
        );
    }

    /**
     * @brief Method for connecting to a client, works asynchronously.
     */
    void waitForClientConnection()
    {
        _acceptor.async_accept([this](const std::error_code error, tcp::socket socket) 
            { acceptingClientConnection(error, socket); });
    }

    /**
     * @bried Method for accepting to a client, works asynchronously.
     */
    void acceptingClientConnection(const std::error_code& error, asio::ip::tcp::socket& socket)
    {
        if (!error)
        {
            std::ostringstream outputPoint;
            outputPoint << socket.remote_endpoint();

            FileLogger::getInstance().log
            (
                "[SERVER] New Connection: " + outputPoint.str(),
                LogLevel::INFO
            );

            auto newConnection = 
                std::make_shared<Connection>(Connection::OwnerType::SERVER, _context, std::move(socket), _incomingMessagesQueue);

            if (onClientConnect(newConnection))
            {
                _connectionsPointers.emplace_back(std::move(newConnection));
                _connectionsPointers.back()->connectToClient(_idCounter++);

                FileLogger::getInstance().log
                (
                    "[" + std::to_string(_connectionsPointers.back()->getID()) 
                    + "] Connection Approved", 
                    LogLevel::INFO
                );
            }
            else
            {
                FileLogger::getInstance().log
                (
                    "[SERVER] Connection Denied", 
                    LogLevel::INFO
                );
            }
        }
        else
        {
            FileLogger::getInstance().log
            (
                "[SERVER] New Connection Error: " + error.message(), 
                LogLevel::ERR
            );
        }

        waitForClientConnection();
    }

    /**
     * @brief Method for sending the message from client.
     * @param Connection management class as std::shared_ptr<Network::Connection>& and Network::Message& class.
     */
    void messageClient(std::shared_ptr<Connection> client, const Message& message)
    {
        if (client != nullptr && client->isConnected())
        {
            client->send(message);
        }
        else
        {
            onClientDisconnect(client);

            client.reset();

            _connectionsPointers.erase(std::remove(_connectionsPointers.begin(), _connectionsPointers.end(), client),
                                       _connectionsPointers.end());
        }
    }

    /**
     * @brief Method of sending a message to all clients.
     * @param Network::Message& class and connection management class as std::shared_ptr<Network::Connection>&.
     */
    void messageAllClients(const Message& message, const std::shared_ptr<Connection>& exceptionClient = nullptr)
    {
        bool deadConnectionExist = false;

        for (auto& client : _connectionsPointers)
        {
            if (client != nullptr && client->isConnected())
            {
                if (client != exceptionClient)
                {
                    client->send(message);
                }
            }
            else
            {
                onClientDisconnect(client);

                client.reset();

                deadConnectionExist = true;
            }
        }
        if (deadConnectionExist)
        {
            _connectionsPointers.erase(std::remove(_connectionsPointers.begin(), _connectionsPointers.end(), nullptr),
                                   _connectionsPointers.end());
        }
    }

    /**
     * @brief Method for updating messages.
     * @param std::size_t limit and bool for method wait() in SafeQueue.
     */
    void update(std::size_t maxMessages = std::numeric_limits<size_t>::max(), bool wait = true) 
    {
        if (wait) _incomingMessagesQueue.wait();

        if (_incomingMessagesQueue.size() > _criticalQueueSize)
        {
            for (size_t i = 0; i < _newThreadsCount; ++i)
            {
                _threads.emplace_back(std::thread([this]() { _context.run(); }));
            }
        }

        size_t messagesCount = size_t();

        while (messagesCount < maxMessages && !_incomingMessagesQueue.empty())
        {
            Message messageInfo = _incomingMessagesQueue.pop_front();

            onMessage(messageInfo.mRemote, messageInfo);
            messagesCount++;
        }
    }

    /**
     * @brief The method for checking of server ping.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void checkServerPing(const std::shared_ptr<Connection>& client, const Message& message) const 
    {
        std::tm formattedTimestamp = UtilityTime::safe_localtime(message.mHeader.mTimestamp);

        std::ostringstream timeOutput;
        timeOutput << std::put_time(&formattedTimestamp, "%F %T");

        FileLogger::getInstance().log
        (
            "[" + timeOutput.str() + "][" + std::to_string(client->getID()) + "]: Server Ping",
                                                LogLevel::INFO
        );

        client->send(message);
    }

    /**
     * @brief The method for checking of receiving all messages by the client.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void readAllMessage(const std::shared_ptr<Connection>& client, const Message& message)
    {
        std::tm formattedTimestamp = UtilityTime::safe_localtime(message.mHeader.mTimestamp);

        std::ostringstream timeOutput;
        timeOutput << std::put_time(&formattedTimestamp, "%F %T");

        FileLogger::getInstance().log
        (
            "[" + timeOutput.str() + "][" + std::to_string(client->getID()) + "]: Message All\n",
                                                LogLevel::INFO
        );
   
        Message messageHandler;
        messageHandler.mHeader.mMessageType = Message::MessageType::ServerMessage;
    
        // msg << client->getID();
        messageAllClients(messageHandler, client);
    }

    /**
     * @brief The method of checking for a channel list.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     */
    void channelListRequest(const std::shared_ptr<Connection>& client) const
    {
        auto futureResult = _repoManager->pushRequest(&IChannelsRepository::getAllChannelsList);

        Message messageHandler;
        messageHandler.mHeader.mMessageType = Message::MessageType::ChannelListRequest;

        auto channelList     = futureResult.get();
        messageHandler.mBody = std::make_any<std::vector<Models::ChannelInfo>>(channelList);

        client->send(messageHandler);
    }

    /**
     * @brief The method for checking message history extraction.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void messageHistoryRequest(const std::shared_ptr<Connection>& client, Message& message) const
    {
        auto channelID = std::any_cast<std::uint64_t>(message.mBody);

        auto futureResult = _repoManager->pushRequest(&IMessagesRepository::getMessageHistory, DataAccess::fmt(channelID));

        Message messageHandler;
        messageHandler.mHeader.mMessageType = Message::MessageType::MessageHistoryAnswer;

        auto messageHistory = futureResult.get();

        messageHandler.mBody = std::make_any<std::vector<Models::MessageInfo>>(messageHistory);
        client->send(messageHandler);
    }

    /**
     * @brief The method for checking the retrieval of stored messages.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void messageStoreRequest(const std::shared_ptr<Connection>& client, Message& message) const 
    {
        auto messageInfo      = std::any_cast<Models::MessageInfo>(message.mBody);
        messageInfo._senderID = client->getUserID();
        messageInfo._message  = Utility::removeSpaces(messageInfo._message);
        messageInfo._time     = UtilityTime::millisecondsSinceEpoch();

        auto futureResult = _repoManager->pushRequest(&IMessagesRepository::storeMessage, fmt(messageInfo));

        Message answerForClient;
        answerForClient.mHeader.mMessageType = Message::MessageType::MessageStoreAnswer;

        auto messageStoringCode = futureResult.get();

        answerForClient.mBody = std::make_any<Utility::StoringMessageCodes>(messageStoringCode);
        client->send(answerForClient);
    }

    /**
     * @brief The method for checking answer history extraction.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void replyHistoryRequest(const std::shared_ptr<Connection>& client, Message& message) const
    {
        auto channelID = std::any_cast<std::uint64_t>(message.mBody);

        auto futureResult = _repoManager->pushRequest(&IRepliesRepository::getReplyHistory, fmt(channelID));

        Message replyMsg;
        replyMsg.mHeader.mMessageType = Message::MessageType::ReplyHistoryAnswer;

        auto replyHistory = futureResult.get();

        replyMsg.mBody = std::make_any<std::vector<Models::ReplyInfo>>(replyHistory);
        client->send(replyMsg);
    }

    /**
     * @brief The method for checking repeatly the retrieval of stored messages.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void replyStoreRequest(const std::shared_ptr<Connection>& client, Message& message) const
    {
        auto replyInfo      = std::any_cast<Models::ReplyInfo>(message.mBody);
        replyInfo._senderID = client->getUserID();
        replyInfo._message  = Utility::removeSpaces(replyInfo._message);

        auto futureResult = _repoManager->pushRequest(&IRepliesRepository::storeReply, fmt(replyInfo));

        Message answerForClient;
        answerForClient.mHeader.mMessageType = Message::MessageType::ReplyStoreAnswer;

        auto replyStoringCode = futureResult.get();

        answerForClient.mBody = std::make_any<Utility::StoringReplyCodes>(replyStoringCode);
        client->send(answerForClient);
    }

    /**
     * @brief The method for checking of deletion a message request.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void messageDeleteRequest(const std::shared_ptr<Connection>& client, Message& message) const
    {
        auto messageInfo      = std::any_cast<Models::MessageInfo>(message.mBody);
        messageInfo._senderID = client->getUserID();

        auto futureResult = _repoManager->pushRequest(&IMessagesRepository::deleteMessage, fmt(messageInfo));

        Message answerForClient;
        answerForClient.mHeader.mMessageType = Message::MessageType::MessageDeleteAnswer;

        const auto deletingMessageCode = futureResult.get();
        answerForClient.mBody          = std::make_any<Utility::DeletingMessageCodes>(deletingMessageCode);
        client->send(answerForClient);
    }

    /**
     * @brief The method for checking the editing of a query.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void messageEditRequest(const std::shared_ptr<Connection>& client, Message& message) const 
    {
        auto messageInfo      = std::any_cast<Models::MessageInfo>(message.mBody);
        messageInfo._senderID = client->getUserID();

        auto futureResult = _repoManager->pushRequest(&IMessagesRepository::editMessage, fmt(messageInfo));

        Message answerForClient;
        answerForClient.mHeader.mMessageType = Message::MessageType::MessageEditAnswer;

        const auto editingMessageCode = futureResult.get();
        answerForClient.mBody         = std::make_any<Utility::EditingMessageCodes>(editingMessageCode);
        client->send(answerForClient);
    }

    /**
     * @brief The method for checking the reaction.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void messageReactionRequest(const std::shared_ptr<Connection>& client, Message& message) const
    {
        auto messageInfo      = std::any_cast<Models::MessageInfo>(message.mBody);
        messageInfo._senderID = client->getUserID();

        auto futureResult = _repoManager->pushRequest(&IMessagesRepository::updateMessageReactions, fmt(messageInfo));

        Message answerForClient;
        answerForClient.mHeader.mMessageType = Message::MessageType::MessageReactionAnswer;

        const auto reactionMessageCode = futureResult.get();
        answerForClient.mBody          = std::make_any<Utility::ReactionMessageCodes>(reactionMessageCode);
        client->send(answerForClient);

    }

    /**
     * @brief The method for checking the registration request.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void registrationRequest(const std::shared_ptr<Connection>& client, Message& message) const
    {
        auto replyInfo = std::any_cast<Models::RegistrationInfo>(message.mBody);

        auto futureResult = _repoManager->pushRequest(&IRegisterRepository::registerUser, fmt(replyInfo));

        Message messageToClient;
        messageToClient.mHeader.mMessageType = Message::MessageType::RegistrationAnswer;

        auto registrationCode = futureResult.get();

        messageToClient.mBody = std::make_any<Utility::RegistrationCodes>(registrationCode);
        client->send(messageToClient);
    }

    /**
     * @brief The method for checking the logging request.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void loginRequest(const std::shared_ptr<Connection>& client, Message& message) const
    {
        auto loginInfo = std::any_cast<Models::LoginInfo>(message.mBody);

        auto futureResult = _repoManager->pushRequest(&ILoginRepository::loginUser, fmt(loginInfo));

        auto userID          = futureResult.get();
        auto loginSuccessful = userID != 0;

        FileLogger::getInstance().log
        (
            "DEBUG: userID=" + std::to_string(userID), 
            LogLevel::DEBUG
        );

        Message messageToClient;
        messageToClient.mHeader.mMessageType = Message::MessageType::LoginAnswer;
        messageToClient.mBody                = std::make_any<bool>(loginSuccessful);

        client->send(messageToClient);

        if (loginSuccessful)
        {
            client->setUserID(userID);

            FileLogger::getInstance().log
            (
                "User " + std::to_string(userID) + " logged in.", 
                LogLevel::INFO
            );
        }
    }

    /**
     * @brief The method for checking the exit from the channel.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void channelLeaveRequest(const std::shared_ptr<Connection>& client, Message& message) const
    {
        Models::ChannelLeaveInfo channelLeaveInfo;

        auto channelName              = std::any_cast<std::string>(message.mBody);
        channelLeaveInfo._creatorID   = client->getUserID();
        channelLeaveInfo._channelName = channelName;

        auto futureResult = _repoManager->pushRequest(&IChannelsRepository::leaveChannel, fmt(channelLeaveInfo));

        Message messageToClient;
        messageToClient.mHeader.mMessageType = Message::MessageType::ChannelLeaveAnswer;

        auto subscribingChannelCodes = futureResult.get();
        messageToClient.mBody        = std::make_any<Utility::ChannelLeaveCodes>(subscribingChannelCodes);
        client->send(messageToClient);
    }

    /**
     * @brief The method for verifying a channel subscription.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void channelSubscribeRequest(const std::shared_ptr<Connection>& client, Message& message) const
    {
        auto channelInfo    = std::any_cast<Models::ChannelSubscriptionInfo>(message.mBody);
        channelInfo._userID = client->getUserID();

        auto futureResult = _repoManager->pushRequest(&IChannelsRepository::subscribeToChannel, fmt(channelInfo));

        Message messageToClient;
        messageToClient.mHeader.mMessageType = Message::MessageType::ChannelSubscribeAnswer;

        auto subscribingChannelCodes = futureResult.get();

        messageToClient.mBody = std::make_any<Utility::ChannelSubscribingCodes>(subscribingChannelCodes);
        client->send(messageToClient);
    }

    /**
     * @brief The method for checking the verification of users subscribed to a channel.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     */
    void channelSubscriptionListRequest(const std::shared_ptr<Connection>& client) const
    {
        const auto userID = client->getUserID();

        auto futureResult = _repoManager->pushRequest(&IChannelsRepository::getChannelSubscriptionList, fmt(userID));

        Message messageToClient;
        messageToClient.mHeader.mMessageType = Message::MessageType::ChannelSubscriptionListAnswer;

        auto subscribingChannelCodes = futureResult.get();
        messageToClient.mBody        = std::make_any<std::vector<uint64_t>>(subscribingChannelCodes);
        client->send(messageToClient);
    }

    /**
     * @brief The method of checking the deletion of a channel.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void channelDeleteRequest(const std::shared_ptr<Connection>& client, Message& message) const
    {
        Models::ChannelDeleteInfo channelDeleteInfo;

        auto channelName               = std::any_cast<std::string>(message.mBody);
        channelDeleteInfo._creatorID   = client->getUserID();
        channelDeleteInfo._channelName = channelName;

        auto futureResult = _repoManager->pushRequest(&IChannelsRepository::deleteChannel, fmt(channelDeleteInfo));

        Message messageToClient;
        messageToClient.mHeader.mMessageType = Message::MessageType::ChannelDeleteAnswer;

        auto deletedChannelCodes = futureResult.get();
        messageToClient.mBody    = std::make_any<Utility::ChannelDeleteCode>(deletedChannelCodes);
        client->send(messageToClient);
    }

    /**
     * @brief The method of checking the creation of a channel.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void channelCreateRequest(const std::shared_ptr<Connection>& client, Message& message) const 
    {
        Models::ChannelInfo newChannelInfo;

        auto channelName            = std::any_cast<std::string>(message.mBody);
        newChannelInfo._creatorID   = client->getUserID();
        newChannelInfo._channelName = channelName;

        auto futureResult = _repoManager->pushRequest(&IChannelsRepository::createChannel, fmt(newChannelInfo));

        Message messageToClient;
        messageToClient.mHeader.mMessageType = Message::MessageType::ChannelCreateAnswer;

        auto channelCreateCode = futureResult.get();
        messageToClient.mBody  = std::make_any<Utility::ChannelCreateCodes>(channelCreateCode);
        client->send(messageToClient);
    }

    /**
     * @brief The method for checking direct messages received by the user.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void directMessageCreateRequest(const std::shared_ptr<Connection>& client, Message& message) const
    {
        auto secondUser = std::any_cast<std::uint64_t>(message.mBody);

        auto futureResult = _repoManager->pushRequest(&IDirectMessageRepository::addDirectChat, fmt(client->getUserID()), fmt(secondUser));

        Message messageToClient;
        messageToClient.mHeader.mMessageType = Message::MessageType::DirectMessageCreateAnswer;

        messageToClient.mBody = std::make_any<Utility::DirectMessageStatus>(futureResult.get());
        client->send(messageToClient);
    }

    /**
     * @brief The method for getting default request.
     */
    void defaultRequest() const
    {
        FileLogger::getInstance().log
        (
            "Unknown command received", 
            LogLevel::ERR
        );
    }
};
}  // namespace Server
