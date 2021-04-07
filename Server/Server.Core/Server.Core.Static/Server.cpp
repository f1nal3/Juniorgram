#include "Server.hpp"

#include <Network/Primitives.hpp>
#include <future>

#include "DataAccess.Static/PostgreRepository.hpp"

using Network::Connection;
using Network::Message;
using Network::SafeQueue;

namespace Server
{
bool Server::onClientConnect(const std::shared_ptr<Connection>& client)
{
    Network::Message message;
    message.mHeader.mID = Network::Message::MessageType::ServerAccept;
    client->send(message);
    return true;
}

void Server::onClientDisconnect(const std::shared_ptr<Connection>& client)
{
    std::cout << "Removing client [" << client->getID() << "]\n";
}

void Server::onMessage(const std::shared_ptr<Connection>& client, Message& message)
{
    const auto maxDelay    = std::chrono::milliseconds(300);
    const auto currentTime = std::chrono::system_clock::now();
    const auto delay =
        std::chrono::milliseconds(std::abs(std::chrono::duration_cast<std::chrono::milliseconds>(
                                               currentTime - message.mHeader.mTimestamp)
                                               .count()));

    if (delay > maxDelay)
    {
        message.mHeader.mTimestamp = currentTime;
    }

    switch (message.mHeader.mID)
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
            msg.mHeader.mID = Network::Message::MessageType::ServerMessage;
            msg << client->getID();
            messageAllClients(msg, client);
        }
        break;

        case Network::Message::MessageType::ChannelListRequest:
        {
            auto future =
                std::async(std::launch::async, &DataAccess::IRepository::getAllChannelsList,
                           _postgreRepo.get());

            Network::Message msg;
            msg.mHeader.mID = Network::Message::MessageType::ChannelListRequest;

            future.wait();
            auto channelList = future.get();
            for (auto& channel : channelList)
            {
                Network::ChannelInfo info;
                info.channelID = 0;

                suppressWarning(4996, -Winit - self) 
                    strcpy(info.channelName, channel.data());
                restoreWarning

                msg << info;
                std::cout << channel << '\n';
            }
            msg << channelList.size();

            client->send(msg);
        }
        break;

        case Network::Message::MessageType::MessageHistoryRequest:
        {
            auto future =
                std::async(std::launch::async, &DataAccess::IRepository::getMessageHistoryForUser,
                           _postgreRepo.get(), std::to_string(client->getID()));

            Network::Message msg;
            msg.mHeader.mID = Network::Message::MessageType::MessageHistoryRequest;

            future.wait();
            auto messageHistory = future.get();

            for (auto& msgFromHistory : messageHistory)
            {
                Network::MessageInfo info;
                info.userID = client->getID();

                suppressWarning(4996, -Werror = pragmas) 
                    strcpy(info.message, msgFromHistory.data());
                restoreWarning

                msg << info;
                std::cout << info.message << '\n';
            }
            msg << messageHistory.size();

            client->send(msg);
        }
        break;

        case Network::Message::MessageType::MessageStoreRequest:
        {
            auto future = std::async(std::launch::async, &DataAccess::IRepository::storeMessage,
                                     _postgreRepo.get(), message);

            future.wait();

            client->send(message);
        }
        break;

        default:
        {
        }
        break;
    }
}

Server::Server(const uint16_t& port)
    : mAcceptor(mContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
      _postgreRepo(new DataAccess::PostgreRepository)
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
    {
        mIncomingMessagesQueue.wait();
    }

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
