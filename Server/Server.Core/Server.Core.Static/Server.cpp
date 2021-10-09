#include "Server.hpp"
#include "Network/Primitives.hpp"
#include "DataAccess.Postgre/PostgreRepositoryManager.hpp"

#include <future>

using Network::Connection;
using Network::Message;
using Network::SafeQueue;

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
    std::cout << "Removing client [" << client->getID() << "]\n";
}

void Server::onMessage(const std::shared_ptr<Connection>& client, Message& message)
{
    using namespace DataAccess;

    const auto maxDelay    = std::chrono::milliseconds(300);
    const auto currentTime = std::chrono::system_clock::now();
    const auto delay =
        std::chrono::milliseconds(std::abs(std::chrono::duration_cast<std::chrono::milliseconds>(
                                               currentTime - message.mHeader.mTimestamp)
                                               .count()));

    if (delay > maxDelay)
        message.mHeader.mTimestamp = currentTime;

    switch (message.mHeader.mMessageType)
    {
        case Network::Message::MessageType::ServerPing:
        {
            std::tm formattedTimestamp = Utility::safe_localtime(
                std::chrono::system_clock::to_time_t(message.mHeader.mTimestamp));

            std::cout << "[" << std::put_time(&formattedTimestamp, "%F %T") << "]["
                      << client->getID() << "]: Server Ping\n";

            client->send(message);
        }
        break;

        case Network::Message::MessageType::MessageAll:
        {
            std::tm formattedTimestamp = Utility::safe_localtime(
                std::chrono::system_clock::to_time_t(message.mHeader.mTimestamp));

            std::cout << "[" << std::put_time(&formattedTimestamp, "%F %T") << "]["
                      << client->getID() << "]: Message All\n";

            Network::Message msg;  // TODO: Why is a new message needed here?
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
            msg.mBody = std::make_any<std::vector<Network::ChannelInfo>>(channelList);

            client->send(msg);
        }
        break;

        case Network::Message::MessageType::MessageHistoryRequest:
        {
            std::uint64_t channelID = std::any_cast<std::uint64_t>(message.mBody);

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
            auto ri = std::any_cast<Network::ReplyInfo>(message.mBody);
            ri.senderID = client->getUserID();

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

            auto userID = future.get();
            auto loginSuccessful = userID != 0;
            
            std::cout << "DEBUG: userID=" << userID << "\n";
            
            Network::Message messageToClient;
            messageToClient.mHeader.mMessageType = Network::Message::MessageType::LoginAnswer;
            messageToClient.mBody = std::make_any<bool>(loginSuccessful);

            client->send(messageToClient);

            if (loginSuccessful)
            {
                client->setUserID(userID);
                std::cout << "User " << userID << " logged in.\n";
            }
            else
            {
                client->disconnect();
            }
        }
        break;

        case Network::Message::MessageType::ChannelDeleteRequest:
        {
            Network::ChannelDeleteInfo chennelDeletedInfo;
            std::string                channelName = std::any_cast<std::string>(message.mBody);
            chennelDeletedInfo.creatorID           = client->getUserID();
            chennelDeletedInfo.channelName         = channelName;

            auto future = mPostgreManager->pushRequest(&IChannelsRepository::deleteChannel, fmt(chennelDeletedInfo));

            Network::Message messageToClient;
            messageToClient.mHeader.mMessageType = Network::Message::MessageType::ChannelDeleteAnswer;

            auto deletedChannelCodes = future.get();
            messageToClient.mBody    = std::make_any<Utility::ChannelDeleteCode>(deletedChannelCodes);
        }
        break;
        
        case Network::Message::MessageType::ChannelCreateRequest:
        {
            Network::ChannelInfo newChennelInfo;
            std::string          channelName = std::any_cast<std::string>(message.mBody);
            newChennelInfo.creatorID         = client->getUserID();
            newChennelInfo.channelName       = channelName;

            auto future = mPostgreManager->pushRequest(&IChannelsRepository::createChannel, fmt(newChennelInfo));

            Network::Message messageToClient;
            messageToClient.mHeader.mMessageType = Network::Message::MessageType::ChannelCreateAnswer;

            auto channelCreateCode = future.get();
            messageToClient.mBody  = std::make_any<Utility::ChannelCreateCodes>(channelCreateCode);
            client->send(messageToClient);
        }
        break;

        default:
        {
            std::cerr << "Unknows command received\n";
        }
        break;
    }
}

Server::Server(const uint16_t& port)
    : mAcceptor(mContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
      mPostgreManager(std::make_unique<DataAccess::PostgreReposiotoryManager>(DataAccess::PostgreAdapter::getInstance<DataAccess::PostgreAdapter>()))
{
}

Server::~Server() { stop(); }

bool Server::start()
{
    try
    {
        waitForClientConnection();

        size_t threadsCount                              = std::thread::hardware_concurrency();
        threadsCount > 1 ? --threadsCount : threadsCount = 1;

        for (size_t i = 0; i < threadsCount; ++i)
        {
            mThreads.emplace_back(std::thread([this]() { mContext.run(); }));
        }

        mPostgreManager->handleRequests();

        std::cout << "[SERVER] Started!\n";
        return true;
    }
    catch (std::exception& exception)
    {
        std::cerr << "[SERVER] Exception: " << exception.what() << "\n";
        return false;
    }
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

    std::cout << "[SERVER] Stopped!\n";
}

void Server::waitForClientConnection()
{
    mAcceptor.async_accept([this](std::error_code error, asio::ip::tcp::socket socket) {
        if (!error)
        {
            std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";

            std::shared_ptr<Connection> newConnection = std::make_shared<Connection>(
                Connection::OwnerType::SERVER, mContext, std::move(socket), mIncomingMessagesQueue);

            if (onClientConnect(newConnection))
            {
                mConnectionsPointers.push_back(std::move(newConnection));

                mConnectionsPointers.back()->connectToClient(mIDCounter++);

                std::cout << "[" << mConnectionsPointers.back()->getID()
                          << "] Connection Approved\n";
            }
            else
            {
                std::cout << "[-----] Connection Denied\n";
            }
        }
        else
        {
            std::cout << "[SERVER] New Connection Error: " << error.message() << "\n";
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

        mConnectionsPointers.erase(
            std::remove(mConnectionsPointers.begin(), mConnectionsPointers.end(), client),
            mConnectionsPointers.end());
    }
}

void Server::messageAllClients(const Message& message,
                               const std::shared_ptr<Connection>& exceptionClient)
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
        mConnectionsPointers.erase(
            std::remove(mConnectionsPointers.begin(), mConnectionsPointers.end(), nullptr),
            mConnectionsPointers.end());
    }
}

void Server::update(size_t maxMessages, bool wait)
{
    if (wait)
        mIncomingMessagesQueue.wait();

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
