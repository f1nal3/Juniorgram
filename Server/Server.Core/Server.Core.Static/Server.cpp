#include "Server.hpp"

#include <future>

#include "DataAccess.Postgre/PostgreRepositories.hpp"
#include "DataAccess.Postgre/PostgreRepositoryContainer.hpp"
#include "Network/Primitives.hpp"

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

void Server::onClientDisconnect(const std::shared_ptr<Connection>& client) { std::cout << "Removing client [" << client->getID() << "]\n"; }

void Server::onMessage(const std::shared_ptr<Connection>& client, Message& message)
{
    const auto maxDelay    = std::chrono::milliseconds(300);
    const auto currentTime = std::chrono::system_clock::now();
    const auto delay       = std::chrono::milliseconds(
              std::abs(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - message.mHeader.mTimestamp).count()));

    if (delay > maxDelay) message.mHeader.mTimestamp = currentTime;

    switch (message.mHeader.mMessageType)
    {
        case Network::Message::MessageType::ServerPing:
        {
            std::tm formattedTimestamp = Utility::safe_localtime(std::chrono::system_clock::to_time_t(message.mHeader.mTimestamp));

            std::cout << "[" << std::put_time(&formattedTimestamp, "%F %T") << "][" << client->getID() << "]: Server Ping\n";

            client->send(message);
        }
        break;

        case Network::Message::MessageType::MessageAll:
        {
            std::tm formattedTimestamp = Utility::safe_localtime(std::chrono::system_clock::to_time_t(message.mHeader.mTimestamp));

            std::cout << "[" << std::put_time(&formattedTimestamp, "%F %T") << "][" << client->getID() << "]: Message All\n";

            Network::Message msg;  // TODO: Why is a new message needed here?
            msg.mHeader.mMessageType = Network::Message::MessageType::ServerMessage;
            // msg << client->getID();

            messageAllClients(msg, client);
        }
        break;

        case Network::Message::MessageType::ChannelListRequest:
        {
            auto IChannelRep = mPostgreRepo->getRepository<DataAccess::IChannelsRepository>();
            auto future      = std::async(std::launch::async, &DataAccess::IChannelsRepository::getAllChannelsList, IChannelRep);

            Network::Message msg;
            msg.mHeader.mMessageType = Network::Message::MessageType::ChannelListRequest;

            auto channelList = future.get();
            msg.mBody        = std::make_any<std::vector<Network::ChannelInfo>>(channelList);

            client->send(msg);
        }
        break;

        case Network::Message::MessageType::MessageHistoryRequest:
        {
            const auto channelID = std::any_cast<std::uint64_t>(message.mBody);

            auto IMessageRep = mPostgreRepo->getRepository<DataAccess::IMessagesRepository>();
            auto future      = std::async(std::launch::async, &DataAccess::IMessagesRepository::getMessageHistory, IMessageRep, channelID);

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

            auto IMessageRep = mPostgreRepo->getRepository<DataAccess::IMessagesRepository>();
            auto future      = std::async(std::launch::async, &DataAccess::IMessagesRepository::storeMessage, IMessageRep, mi);

            Network::Message answerForClient;
            answerForClient.mHeader.mMessageType = Network::Message::MessageType::MessageStoreAnswer;

            auto messageStoringCode = future.get();

            answerForClient.mBody = std::make_any<Utility::StoringMessageCodes>(messageStoringCode);
            client->send(answerForClient);
        }
        break;

        case Network::Message::MessageType::ReplyHistoryRequest:
        {
            const auto channelID = std::any_cast<std::uint64_t>(message.mBody);

            auto IReplyRep = mPostgreRepo->getRepository<DataAccess::IRepliesRepository>();
            auto future    = std::async(std::launch::async, &DataAccess::IRepliesRepository::getReplyHistory, IReplyRep, channelID);

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

            auto IReplyRep = mPostgreRepo->getRepository<DataAccess::IRepliesRepository>();
            auto future    = std::async(std::launch::async, &DataAccess::IRepliesRepository::storeReply, IReplyRep, ri);

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

            auto IMessageRep = mPostgreRepo->getRepository<DataAccess::IMessagesRepository>();
            auto future      = std::async(std::launch::async, &DataAccess::IMessagesRepository::deleteMessage, IMessageRep, mi);

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

            auto IRegisterRep = mPostgreRepo->getRepository<DataAccess::IRegisterRepository>();
            auto future       = std::async(std::launch::async, &DataAccess::IRegisterRepository::registerUser, IRegisterRep, ri);

            Network::Message messageToClient;
            messageToClient.mHeader.mMessageType = Network::Message::MessageType::RegistrationAnswer;

            auto registrationCode = future.get();

            messageToClient.mBody = std::make_any<Utility::RegistrationCodes>(registrationCode);
            client->send(messageToClient);
        }
        break;

        case Network::Message::MessageType::LoginRequest:
        {
            Network::LoginInfo loginInfo = std::any_cast<Network::LoginInfo>(message.mBody);

            auto ILoginRep = mPostgreRepo->getRepository<DataAccess::ILoginRepository>();
            auto future =
                std::async(std::launch::async, &DataAccess::ILoginRepository::loginUser, ILoginRep, loginInfo.login, loginInfo.pwdHash);

            auto userID          = future.get();
            auto loginSuccessful = userID != 0;

            std::cout << "DEBUG: userID=" << userID << "\n";

            Network::Message messageToClient;
            messageToClient.mHeader.mMessageType = Network::Message::MessageType::LoginAnswer;
            messageToClient.mBody                = std::make_any<bool>(loginSuccessful);

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

        case Network::Message::MessageType::ChannelLeaveRequest:
        {
            Network::ChannelLeaveInfo leavedChennelInfo;
            std::string               channelName = std::any_cast<std::string>(message.mBody);
            leavedChennelInfo.creatorID           = client->getUserID();
            leavedChennelInfo.channelName         = channelName;

            auto IChannelRep = mPostgreRepo->getRepository<DataAccess::IChannelsRepository>();
            auto future = std::async(std::launch::async, &DataAccess::IChannelsRepository::leaveChannel, IChannelRep, leavedChennelInfo);

            Network::Message messageToClient;
            messageToClient.mHeader.mMessageType = Network::Message::MessageType::ChannelLeaveAnswer;

            auto subscribingChannelCodes = future.get();
            messageToClient.mBody        = std::make_any<Utility::ChannelLeaveCodes>(subscribingChannelCodes);
        }
        case Network::Message::MessageType::ChannelSubscribeRequest:
        {
            auto channel = std::any_cast<Network::ChannelSubscriptionInfo>(message.mBody);
            channel.userID = client->getUserID();

            auto IChannelRep = mPostgreRepo->getRepository<DataAccess::IChannelsRepository>();
            auto future      = std::async(std::launch::async, &DataAccess::IChannelsRepository::subscribeToChannel, IChannelRep, channel);

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

            auto IChannelRep = mPostgreRepo->getRepository<DataAccess::IChannelsRepository>();
            auto future = std::async(std::launch::async, &DataAccess::IChannelsRepository::getChannelSubscriptionList, IChannelRep, userID);

            Network::Message messageToClient;
            messageToClient.mHeader.mMessageType = Network::Message::MessageType::ChannelSubscriptionListAnswer;

            auto subscribingChannelCodes = future.get();
            messageToClient.mBody =
                std::make_any<std::vector<uint64_t>>(subscribingChannelCodes);
        }
        break;
        
        case Network::Message::MessageType::ChannelDeleteRequest:
        {
            Network::ChannelDeleteInfo chennelDeletedInfo;
            std::string                channelName = std::any_cast<std::string>(message.mBody);
            chennelDeletedInfo.creatorID           = client->getUserID();
            chennelDeletedInfo.channelName         = channelName;

            auto IChannelRep = mPostgreRepo->getRepository<DataAccess::IChannelsRepository>();
            auto future = std::async(std::launch::async, &DataAccess::IChannelsRepository::deleteChannel, IChannelRep, chennelDeletedInfo);

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

            auto IChannelRep = mPostgreRepo->getRepository<DataAccess::IChannelsRepository>();
            auto future      = std::async(std::launch::async, &DataAccess::IChannelsRepository::createChannel, IChannelRep, newChennelInfo);

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
      mPostgreRepo(
          std::make_unique<DataAccess::PostgreRepositoryContainer>(DataAccess::PostgreAdapter::getInstance<DataAccess::PostgreAdapter>()))
{
}

Server::~Server() { stop(); }

bool Server::start()
{
    try
    {
        waitForClientConnection();

        size_t threadsCount = std::thread::hardware_concurrency();
        threadsCount > 1 ? --threadsCount : threadsCount = 1;

        for (size_t i = 0; i < threadsCount; ++i)
        {
            mThreads.emplace_back(std::thread([this]() { mContext.run(); }));
        }

        std::cout << "[SERVER] Started!\n";
        return true;
    }
    catch (std::exception& exception)
    {
        std::cerr << "[SERVER] Exception: " << exception.what() << "\n";
        return false;
    }
}

void Server::registerRepositories()
{
    using namespace DataAccess;

    mPostgreRepo->registerRepository<IChannelsRepository, ChannelsRepository>();
    mPostgreRepo->registerRepository<ILoginRepository, LoginRepository>();
    mPostgreRepo->registerRepository<IMessagesRepository, MessagesRepository>();
    mPostgreRepo->registerRepository<IRegisterRepository, RegisterRepository>();
    mPostgreRepo->registerRepository<IRepliesRepository, RepliesRepository>();
}

void Server::stop()
{
    mContext.stop();

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

            std::shared_ptr<Connection> newConnection =
                std::make_shared<Connection>(Connection::OwnerType::SERVER, mContext, std::move(socket), mIncomingMessagesQueue);

            if (onClientConnect(newConnection))
            {
                mConnectionsPointers.push_back(std::move(newConnection));

                mConnectionsPointers.back()->connectToClient(mIDCounter++);

                std::cout << "[" << mConnectionsPointers.back()->getID() << "] Connection Approved\n";
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
