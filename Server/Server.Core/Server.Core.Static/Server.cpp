#include "Server.hpp"



#include <Network.Static/Connection.hpp>
#include <Network.Static/Primitives.hpp>

#include <Utility.Static/YasSerializer.hpp>
#include <Utility.Static/TokenBuilder.hpp>
#include <Utility.Static/ClientPayload.hpp>

#include <DataAccess.Static/PostgreRepository.hpp>
#include <DataAccess.Static/RepositoryUnits.hpp>


#include <future>

namespace Server
{

bool Server::onClientConnect(const std::shared_ptr<Network::Connection>& client)
{
    Network::Message message;
    message.mHeader.mMessageType = Network::Message::MessageType::ServerAccept;
    client->send(message);

    message.mHeader.mMessageType = Network::Message::MessageType::SetEncryptedConnection;
    message.mBody = std::make_any<std::string>((*client->getKeyDestributor()).getPublicServerKey());
    client->send(message);

    return true;
}

void Server::onClientDisconnect(const std::shared_ptr<Network::Connection>& client)
{
    std::cout << "Removing client [" << client->getID() << "]\n";
}

void Server::onMessage(const std::shared_ptr<Network::Connection>& client, Network::Message& message)
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

        case Network::Message::MessageType::SetEncryptedConnection:
        {
            std::string publicClientKey = std::any_cast<std::string>(message.mBody);

            client->getKeyDestributor().get()->setPublicClientKey(publicClientKey);

            client->getKeyDestributor().get()->calculateSharedSecret();

            message.mHeader.mMessageType = Network::Message::MessageType::SendIV;

            message.mBody = std::make_any<std::string>(Utility::EncryptionHandler::getIVBlock());
            client->send(message);

            std::cout << "Client accept encrypted connection!"; 
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
            auto future =
                std::async(std::launch::async, &DataAccess::IRepository::getAllChannelsList,
                           _postgreRepo.get());

            Network::Message msg;
            msg.mHeader.mMessageType = Network::Message::MessageType::ChannelListRequest;

            future.wait();

            // channelList should be std::vector<Network::ChannelInfo>
            auto channelList = future.get();

            // loop stub for forming std::vector<Network::ChannelInfo>
            std::vector<Network::ChannelInfo> channelInfoList;
            for (auto& channel : channelList)
            {
                Network::ChannelInfo info;
                info.channelID   = 0;
                info.channelName = channel;

                channelInfoList.push_back(info);
            }

            msg.mBody = std::make_any<std::vector<Network::ChannelInfo>>(channelInfoList);

            client->send(msg);
        }
        break;

        case Network::Message::MessageType::MessageHistoryRequest:
        {
            auto future =
                std::async(std::launch::async, &DataAccess::IRepository::getMessageHistoryForUser,
                           _postgreRepo.get(), 0); // There need to add channelID not 0.

            Network::Message msg;
            msg.mHeader.mMessageType = Network::Message::MessageType::MessageHistoryRequest;

            future.wait();
           
            // messageHistory should be std::vector<Network::MessageInfo>
            auto messageHistory = future.get();

            std::vector<Network::MessageInfo> messageHistoryList;
            for (auto& msgFromHistory : messageHistory)
            {
                Network::MessageInfo info;
                info.userID  = client->getID();
                info.message = msgFromHistory.data();

                messageHistoryList.push_back(info);

                std::cout << info.message << '\n';
            }
            msg.mBody = std::make_any<std::vector<Network::MessageInfo>>(messageHistoryList);
            
            client->send(msg);
        }
        break;

        case Network::Message::MessageType::MessageStoreRequest:
        {
            auto msgInfo = std::any_cast<Network::MessageInfo>(message.mBody);
            
            auto future = std::async(std::launch::async, &DataAccess::IRepository::storeMessage,
                           _postgreRepo.get(), msgInfo, 0);  // There need to add channelID not 0.
            
            message.mBody = std::make_any<Network::MessageInfo>(msgInfo);

            future.wait();
            client->send(message);
        }
        break;

        case Network::Message::MessageType::RegistrationRequest:
        {
            auto [clientPayload ,ri] = std::any_cast<std::pair<Utility::ClientPayload, Network::RegistrationInfo>>(message.mBody);
            
            auto future = std::async(std::launch::async, &RegistrationUnit::registerUser, &RegistrationUnit::instance(), ri);
        
            Network::Message messageToClient;
            messageToClient.mHeader.mMessageType =
                Network::Message::MessageType::RegistrationAnswer;
            
            auto registrationCode = future.get();

            if (registrationCode == Utility::RegistrationCodes::SUCCESS)
            {
                std::string token = getToken(/*clientPayload, */client);
                messageToClient.mBody =
                    std::make_any<std::pair<std::string, Utility::RegistrationCodes>>(
                        std::pair{" ", registrationCode});
            }
            else
            {
                messageToClient.mBody = std::make_any<Utility::RegistrationCodes>(registrationCode);
                client->send(messageToClient);
            }
        }
        break;

        default:
        {
            std::cerr << "Unknows command received\n";
        }
        break;
    }
}

std::string Server::getToken(const std::shared_ptr<Network::Connection>& client)
{ 
    std::string token = Utility::buildToken(client);
    return token;
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

        size_t threadsCount                              = std::thread::hardware_concurrency();
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

            std::shared_ptr<Network::Connection> newConnection =
                std::make_shared<Network::Connection>(Network::Connection::OwnerType::SERVER,
                                                      mContext, std::move(socket),
                                                      mIncomingMessagesQueue);

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

void Server::messageClient(std::shared_ptr<Network::Connection> client, const Network::Message& message)
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

void Server::messageAllClients(const Network::Message& message,
                               const std::shared_ptr<Network::Connection>& exceptionClient)
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
        Network::Message message = mIncomingMessagesQueue.pop_front();

        onMessage(message.mRemote, message);

        messagesCount++;
    }
}

}  // namespace Server
