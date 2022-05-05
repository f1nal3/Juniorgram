#include "Server.hpp"

#include <future>

#include "DataAccess.Postgre/PostgreRepositoryManager.hpp"

using Network::Connection;
using Network::Message;
using Network::SafeQueue;
using UtilityTime::safe_localtime;

namespace Server
{
bool Server::onClientConnect(const std::shared_ptr<Connection>& client)
{
    Network::Message message;
    message.mHeader.mMessageType = Network::Message::MessageType::ServerAccept;
    client->send(message);
    return true;
}

void Server::onClientDisconnect(const std::shared_ptr<Connection>& client)
{
    Base::Logger::FileLogger::getInstance().log
    (
        "Removing client [" + std::to_string(client->getID()) + "]", 
        Base::Logger::LogLevel::INFO
    );
}

void Server::onMessage(const std::shared_ptr<Connection>& client, Message& message)
{
    using namespace DataAccess;

    switch (message.mHeader.mMessageType)
    {
        case Network::Message::MessageType::ServerPing:
        {
            std::tm formattedTimestamp = safe_localtime(message.mHeader.mTimestamp);
            
            std::ostringstream out;
            out << std::put_time(&formattedTimestamp, "%F %T");
            
            Base::Logger::FileLogger::getInstance().log
            (
                "[" + out.str() + "][" + std::to_string(client->getID()) + "]: Server Ping", 
                Base::Logger::LogLevel::INFO
            );

            client->send(message);
        }
        break;

        case Network::Message::MessageType::MessageAll:
        {
            std::tm formattedTimestamp = safe_localtime(message.mHeader.mTimestamp);

            std::ostringstream out;
            out << std::put_time(&formattedTimestamp, "%F %T");

            Base::Logger::FileLogger::getInstance().log
            (
                "[" + out.str() + "][" + std::to_string(client->getID()) + "]: Message All\n", 
                Base::Logger::LogLevel::INFO
            );

            Network::Message msg;  // T\todo Why is a new message needed here?
            msg.mHeader.mMessageType = Network::Message::MessageType::ServerMessage;
            // msg << client->getID();

            messageAllClients(msg, client);
        }
        break;

        case Network::Message::MessageType::ChannelListRequest:
        {
            auto future = mPostgreManager->pushRequest(&IChannelsRepository::getAllChannelsList);

            Network::Message msg;
            msg.mHeader.mMessageType = Network::Message::MessageType::ChannelListRequest;

            auto channelList = future.get();
            msg.mBody        = std::make_any<std::vector<Network::ChannelInfo>>(channelList);

            client->send(msg);
        }
        break;

        case Network::Message::MessageType::MessageHistoryRequest:
        {
            auto channelID = std::any_cast<std::uint64_t>(message.mBody);

            auto future = mPostgreManager->pushRequest(&IMessagesRepository::getMessageHistory, fmt(channelID));

            Network::Message msg;
            msg.mHeader.mMessageType = Network::Message::MessageType::MessageHistoryAnswer;

            auto messageHistory = future.get();

            msg.mBody = std::make_any<std::vector<Network::MessageInfo>>(messageHistory);
            client->send(msg);
        }
        break;

        case Network::Message::MessageType::MessageStoreRequest:
        {
            auto mi     = std::any_cast<Network::MessageInfo>(message.mBody);
            mi.senderID = client->getUserID();
            mi.message  = Utility::removeSpaces(mi.message);
            mi.time     = UtilityTime::millisecondsSinceEpoch();

            auto future = mPostgreManager->pushRequest(&IMessagesRepository::storeMessage, fmt(mi));

            Network::Message answerForClient;
            answerForClient.mHeader.mMessageType = Network::Message::MessageType::MessageStoreAnswer;

            auto messageStoringCode = future.get();

            answerForClient.mBody = std::make_any<Utility::StoringMessageCodes>(messageStoringCode);
            client->send(answerForClient);
        }
        break;

        case Network::Message::MessageType::ReplyHistoryRequest:
        {
            auto channelID = std::any_cast<std::uint64_t>(message.mBody);

            auto future = mPostgreManager->pushRequest(&IRepliesRepository::getReplyHistory, fmt(channelID));

            Network::Message replyMsg;
            replyMsg.mHeader.mMessageType = Network::Message::MessageType::ReplyHistoryAnswer;

            auto replyHistory = future.get();

            replyMsg.mBody = std::make_any<std::vector<Network::ReplyInfo>>(replyHistory);
            client->send(replyMsg);
        }
        break;

        case Network::Message::MessageType::ReplyStoreRequest:
        {
            auto ri     = std::any_cast<Network::ReplyInfo>(message.mBody);
            ri.senderID = client->getUserID();
            ri.message  = Utility::removeSpaces(ri.message);

            auto future = mPostgreManager->pushRequest(&IRepliesRepository::storeReply, fmt(ri));

            Network::Message answerForClient;
            answerForClient.mHeader.mMessageType = Network::Message::MessageType::ReplyStoreAnswer;

            auto replyStoringCode = future.get();

            answerForClient.mBody = std::make_any<Utility::StoringReplyCodes>(replyStoringCode);
            client->send(answerForClient);
        }
        break;

        case Network::Message::MessageType::MessageDeleteRequest:
        {
            auto mi     = std::any_cast<Network::MessageInfo>(message.mBody);
            mi.senderID = client->getUserID();

            auto future = mPostgreManager->pushRequest(&IMessagesRepository::deleteMessage, fmt(mi));

            Network::Message answerForClient;
            answerForClient.mHeader.mMessageType = Network::Message::MessageType::MessageDeleteAnswer;

            const auto deletingMessageCode = future.get();
            answerForClient.mBody          = std::make_any<Utility::DeletingMessageCodes>(deletingMessageCode);
            client->send(answerForClient);
        }
        break;

        case Network::Message::MessageType::MessageEditRequest:
        {
            auto mi     = std::any_cast<Network::MessageInfo>(message.mBody);
            mi.senderID = client->getUserID();

            auto future = mPostgreManager->pushRequest(&IMessagesRepository::editMessage, fmt(mi));

            Network::Message answerForClient;
            answerForClient.mHeader.mMessageType = Network::Message::MessageType::MessageEditAnswer;

            const auto editingMessageCode = future.get();
            answerForClient.mBody         = std::make_any<Utility::EditingMessageCodes>(editingMessageCode);
            client->send(answerForClient);
        }
        break;

        case Network::Message::MessageType::MessageReactionRequest:
        {
            auto messageInfo     = std::any_cast<Network::MessageInfo>(message.mBody);
            messageInfo.senderID = client->getUserID();

            auto future = mPostgreManager->pushRequest(&IMessagesRepository::updateMessageReactions, fmt(messageInfo));

            Network::Message answerForClient;
            answerForClient.mHeader.mMessageType = Network::Message::MessageType::MessageReactionAnswer;

            const auto reactionMessageCode = future.get();
            answerForClient.mBody          = std::make_any<Utility::ReactionMessageCodes>(reactionMessageCode);
            client->send(answerForClient);
        }
        break;

        case Network::Message::MessageType::RegistrationRequest:
        {
            auto ri = std::any_cast<Network::RegistrationInfo>(message.mBody);

            auto future = mPostgreManager->pushRequest(&IRegisterRepository::registerUser, fmt(ri));

            Network::Message messageToClient;
            messageToClient.mHeader.mMessageType = Network::Message::MessageType::RegistrationAnswer;

            auto registrationCode = future.get();

            messageToClient.mBody = std::make_any<Utility::RegistrationCodes>(registrationCode);
            client->send(messageToClient);
        }
        break;

        case Network::Message::MessageType::LoginRequest:
        {
            auto li = std::any_cast<Network::LoginInfo>(message.mBody);

            auto future = mPostgreManager->pushRequest(&ILoginRepository::loginUser, fmt(li));

            auto userID          = future.get();
            auto loginSuccessful = userID != 0;

            Base::Logger::FileLogger::getInstance().log
            (
                "DEBUG: userID=" + std::to_string(userID), 
                Base::Logger::LogLevel::DEBUG
            );

            Network::Message messageToClient;
            messageToClient.mHeader.mMessageType = Network::Message::MessageType::LoginAnswer;
            messageToClient.mBody                = std::make_any<bool>(loginSuccessful);

            client->send(messageToClient);

            if (loginSuccessful)
            {
                client->setUserID(userID);

                Base::Logger::FileLogger::getInstance().log
                (
                    "User " + std::to_string(userID) + " logged in.", 
                    Base::Logger::LogLevel::INFO
                );
            }
        }
        break;

        case Network::Message::MessageType::ChannelLeaveRequest:
        {
            Network::ChannelLeaveInfo channelLeaveInfo;

            auto channelName             = std::any_cast<std::string>(message.mBody);
            channelLeaveInfo.creatorID   = client->getUserID();
            channelLeaveInfo.channelName = channelName;

            auto future = mPostgreManager->pushRequest(&IChannelsRepository::leaveChannel, fmt(channelLeaveInfo));

            Network::Message messageToClient;
            messageToClient.mHeader.mMessageType = Network::Message::MessageType::ChannelLeaveAnswer;

            auto subscribingChannelCodes = future.get();
            messageToClient.mBody        = std::make_any<Utility::ChannelLeaveCodes>(subscribingChannelCodes);
            client->send(messageToClient);
        }
        break;

        case Network::Message::MessageType::ChannelSubscribeRequest:
        {
            auto channel   = std::any_cast<Network::ChannelSubscriptionInfo>(message.mBody);
            channel.userID = client->getUserID();

            auto future = mPostgreManager->pushRequest(&IChannelsRepository::subscribeToChannel, fmt(channel));

            Network::Message messageToClient;
            messageToClient.mHeader.mMessageType = Network::Message::MessageType::ChannelSubscribeAnswer;

            auto subscribingChannelCodes = future.get();

            messageToClient.mBody = std::make_any<Utility::ChannelSubscribingCodes>(subscribingChannelCodes);
            client->send(messageToClient);
        }
        break;

        case Network::Message::MessageType::ChannelSubscriptionListRequest:
        {
            const auto userID = client->getUserID();

            auto future = mPostgreManager->pushRequest(&IChannelsRepository::getChannelSubscriptionList, fmt(userID));

            Network::Message messageToClient;
            messageToClient.mHeader.mMessageType = Network::Message::MessageType::ChannelSubscriptionListAnswer;

            auto subscribingChannelCodes = future.get();
            messageToClient.mBody        = std::make_any<std::vector<uint64_t>>(subscribingChannelCodes);
            client->send(messageToClient);
        }
        break;

        case Network::Message::MessageType::ChannelDeleteRequest:
        {
            Network::ChannelDeleteInfo channelDeleteInfo;

            auto channelName              = std::any_cast<std::string>(message.mBody);
            channelDeleteInfo.creatorID   = client->getUserID();
            channelDeleteInfo.channelName = channelName;

            auto future = mPostgreManager->pushRequest(&IChannelsRepository::deleteChannel, fmt(channelDeleteInfo));

            Network::Message messageToClient;
            messageToClient.mHeader.mMessageType = Network::Message::MessageType::ChannelDeleteAnswer;

            auto deletedChannelCodes = future.get();
            messageToClient.mBody    = std::make_any<Utility::ChannelDeleteCode>(deletedChannelCodes);
            client->send(messageToClient);
        }
        break;

        case Network::Message::MessageType::ChannelCreateRequest:
        {
            Network::ChannelInfo newChannelInfo;

            auto channelName           = std::any_cast<std::string>(message.mBody);
            newChannelInfo.creatorID   = client->getUserID();
            newChannelInfo.channelName = channelName;

            auto future = mPostgreManager->pushRequest(&IChannelsRepository::createChannel, fmt(newChannelInfo));

            Network::Message messageToClient;
            messageToClient.mHeader.mMessageType = Network::Message::MessageType::ChannelCreateAnswer;

            auto channelCreateCode = future.get();
            messageToClient.mBody  = std::make_any<Utility::ChannelCreateCodes>(channelCreateCode);
            client->send(messageToClient);
        }
        break;

        case Network::Message::MessageType::DirectMessageCreateRequest:
        {
            auto secondUser = std::any_cast<std::uint64_t>(message.mBody);

            auto result = mPostgreManager->pushRequest(&IDirectMessageRepository::addDirectChat, fmt(client->getUserID()), fmt(secondUser));

            Network::Message messageToClient;
            messageToClient.mHeader.mMessageType = Network::Message::MessageType::DirectMessageCreateAnswer;

            messageToClient.mBody = std::make_any<Utility::DirectMessageStatus>(result.get());
            client->send(messageToClient);
        }
        break;

        default:
        {
            Base::Logger::FileLogger::getInstance().log("Unknown command received", Base::Logger::LogLevel::ERR);
        }
        break;
    }
}

Server::Server(const uint16_t& port)
    : mAcceptor(mContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
      mPostgreManager(
          std::make_unique<DataAccess::PostgreRepositoryManager>(DataAccess::PostgreAdapter::getInstance<DataAccess::PostgreAdapter>()))
{
}

Server::~Server() { stop(); }

void Server::start()
{
    waitForClientConnection();

    size_t threadsCount = std::thread::hardware_concurrency();
    threadsCount > 1 ? --threadsCount : threadsCount = 1;
    
    for (size_t i = 0; i < threadsCount; ++i)
    {
        mThreads.emplace_back(std::thread([this]() { mContext.run(); }));
    }
    mPostgreManager->handleRequests();

    Base::Logger::FileLogger::getInstance().log("[SERVER] Started!", Base::Logger::LogLevel::INFO);
}

void Server::stop()
{
    mContext.stop();

    mPostgreManager->stopHandler();

    for (std::thread& thread : mThreads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }

    mThreads.clear();
    Base::Logger::FileLogger::getInstance().log("[SERVER] Stopped!", Base::Logger::LogLevel::INFO);
}

void Server::waitForClientConnection()
{
    mAcceptor.async_accept([this](std::error_code error, asio::ip::tcp::socket socket) {
        if (!error)
        {
            std::ostringstream out;
            out << socket.remote_endpoint();
            Base::Logger::FileLogger::getInstance().log
            (
                "[SERVER] New Connection: " + out.str(), 
                Base::Logger::LogLevel::INFO
            );

            std::shared_ptr<Connection> newConnection =
                std::make_shared<Connection>(Connection::OwnerType::SERVER, mContext, std::move(socket), mIncomingMessagesQueue);

            /// @todo This function always return true
            if (onClientConnect(newConnection))
            {
                mConnectionsPointers.push_back(std::move(newConnection));

                mConnectionsPointers.back()->connectToClient(mIDCounter++);

                Base::Logger::FileLogger::getInstance().log
                (
                   "[" + std::to_string(mConnectionsPointers.back()->getID()) + "] Connection Approved",
                    Base::Logger::LogLevel::INFO
                );
            }
            else
            {
                Base::Logger::FileLogger::getInstance().log
                (
                    "[-----] Connection Denied", 
                    Base::Logger::LogLevel::INFO
                );
            }
        }
        else
        {
            Base::Logger::FileLogger::getInstance().log
            (
                "[SERVER] New Connection Error: " + error.message(),
                Base::Logger::LogLevel::ERR
            );
        }

        waitForClientConnection();
    });
}

void Server::messageClient(std::shared_ptr<Connection> client, const Message& message)
{
    if (client != nullptr && client->isConnected())
    {
        client->send(message);
    }
    else
    {
        onClientDisconnect(client);

        client.reset();

        mConnectionsPointers.erase(std::remove(mConnectionsPointers.begin(), mConnectionsPointers.end(), client),
                                   mConnectionsPointers.end());
    }
}

void Server::messageAllClients(const Message& message, const std::shared_ptr<Connection>& exceptionClient)
{
    bool deadConnectionExist = false;

    for (auto& client : mConnectionsPointers)
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
        mConnectionsPointers.erase(std::remove(mConnectionsPointers.begin(), mConnectionsPointers.end(), nullptr),
                                   mConnectionsPointers.end());
    }
}

void Server::update(size_t maxMessages, bool wait)
{
    if (wait) mIncomingMessagesQueue.wait();

    if (mIncomingMessagesQueue.size() > mCriticalQueueSize)
    {
        for (size_t i = 0; i < mNewThreadsCount; ++i)
        {
            mThreads.emplace_back(std::thread([this]() { mContext.run(); }));
        }
    }

    size_t messagesCount = size_t();

    while (messagesCount < maxMessages && !mIncomingMessagesQueue.empty())
    {
        Message message = mIncomingMessagesQueue.pop_front();

        onMessage(message.mRemote, message);
        messagesCount++;
    }
}

}  // namespace Server
