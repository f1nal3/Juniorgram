#include <future>

#include "Server.hpp"
#include "DataAccess.Postgre/PostgreRepositoryManager.hpp"

namespace Server
{
using namespace DataAccess;
using Network::Connection;
using Network::Message;
using Network::SafeQueue;
using Base::Logger::FileLogger;
using Base::Logger::LogLevel;
using UtilityTime::safe_localtime;

bool Server::onClientConnect(const std::shared_ptr<Connection>& client)
{
    Network::Message message;
    message.mHeader.mMessageType = Network::Message::MessageType::ServerAccept;
    client->send(message);
    return true;
}

void Server::onClientDisconnect(const std::shared_ptr<Connection>& client)
{
    FileLogger::getInstance().log
    (
        "Removing client [" + std::to_string(client->getID()) + "]", 
        LogLevel::INFO
    );
}

void Server::onMessage(const std::shared_ptr<Connection>& client, Message& message)
{

    switch (message.mHeader.mMessageType)
    {
        case Network::Message::MessageType::ServerPing:
        {
            checkServerPing(client, message);
        }
        break;

        case Network::Message::MessageType::MessageAll:
        {
            readAllMessage(client, message);
        }
        break;

        case Network::Message::MessageType::ChannelListRequest:
        {
            channelListRequest(client);
        }
        break;

        case Network::Message::MessageType::MessageHistoryRequest:
        {
            messageHistoryRequest(client, message);
        }
        break;

        case Network::Message::MessageType::MessageStoreRequest:
        {
            messageStoreRequest(client, message);
        }
        break;

        case Network::Message::MessageType::ReplyHistoryRequest:
        {
            replyHistoryRequest(client, message);
        }
        break;

        case Network::Message::MessageType::ReplyStoreRequest:
        {
            replyStoreRequest(client, message);
        }
        break;

        case Network::Message::MessageType::MessageDeleteRequest:
        {
            messageDeleteRequest(client, message);
        }
        break;

        case Network::Message::MessageType::MessageEditRequest:
        {
            messageEditRequest(client, message);
        }
        break;

        case Network::Message::MessageType::MessageReactionRequest:
        {
            messageReactionRequest(client, message);
        }
        break;

        case Network::Message::MessageType::RegistrationRequest:
        {
            registrationRequest(client, message);
        }
        break;

        case Network::Message::MessageType::LoginRequest:
        {
            loginRequest(client, message);
        }
        break;

        case Network::Message::MessageType::ChannelLeaveRequest:
        {
            channelLeaveRequest(client, message);
        }
        break;

        case Network::Message::MessageType::ChannelSubscribeRequest:
        {
            channelSubscribeRequest(client, message);
        }
        break;

        case Network::Message::MessageType::ChannelSubscriptionListRequest:
        {
            channelSubscriptionListRequest(client);
        }
        break;

        case Network::Message::MessageType::ChannelDeleteRequest:
        {
            channelDeleteRequest(client, message);
        }
        break;

        case Network::Message::MessageType::ChannelCreateRequest:
        {
            channelCreateRequest(client, message);
        }
        break;

        case Network::Message::MessageType::DirectMessageCreateRequest:
        {
            directMessageCreateRequest(client, message);
        }
        break;

        default:
        {
            defaultRequest();
        }
        break;
    }
}
Server::Server(const uint16_t& port) :
     _acceptor(_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
     _postgreManager(std::make_unique<PostgreRepositoryManager>(PostgreAdapter::getInstance<PostgreAdapter>()))
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
        _threads.emplace_back(std::thread([this]() { _context.run(); }));
    }
    _postgreManager->handleRequests();

    FileLogger::getInstance().log
    (
        "[SERVER] Started!", 
        LogLevel::INFO
    );
}

void Server::stop()
{
    _context.stop();

    _postgreManager->stopHandler();

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

void Server::waitForClientConnection()
{   
    _acceptor.async_accept([this](const std::error_code error, asio::ip::tcp::socket socket) { acceptingClientConnection(error, socket); });
}

void Server::acceptingClientConnection(const std::error_code& error, asio::ip::tcp::socket& socket)
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
            _connectionsPointers.back()->connectToClient(_IDCounter++);

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

        _connectionsPointers.erase(std::remove(_connectionsPointers.begin(), _connectionsPointers.end(), client),
                                   _connectionsPointers.end());
    }
}

void Server::messageAllClients(const Message& message, const std::shared_ptr<Connection>& exceptionClient)
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

void Server::update(size_t maxMessages, bool wait)
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
        ++messagesCount;
    }
}

void Server::checkServerPing(const std::shared_ptr<Connection>& client, const Message& message) const
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

void Server::readAllMessage(const std::shared_ptr<Connection>& client, const Message& message)
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

void Server::channelListRequest(const std::shared_ptr<Connection>& client) const
{
    auto futureResult = _postgreManager->pushRequest(&IChannelsRepository::getAllChannelsList);

    Message messageHandler;
    messageHandler.mHeader.mMessageType = Message::MessageType::ChannelListRequest;

    auto channelList     = futureResult.get();
    messageHandler.mBody = std::make_any<std::vector<Network::ChannelInfo>>(channelList);

    client->send(messageHandler);
}

void Server::messageHistoryRequest(const std::shared_ptr<Connection>& client, Message& message) const
{
    auto channelID = std::any_cast<std::uint64_t>(message.mBody);

    auto futureResult = _postgreManager->pushRequest(&IMessagesRepository::getMessageHistory, DataAccess::fmt(channelID));

    Message messageHandler;
    messageHandler.mHeader.mMessageType = Message::MessageType::MessageHistoryAnswer;

    auto messageHistory = futureResult.get();

    messageHandler.mBody = std::make_any<std::vector<Network::MessageInfo>>(messageHistory);
    client->send(messageHandler);
}

void Server::messageStoreRequest(const std::shared_ptr<Connection>& client, Message& message) const
{
    auto messageInfo     = std::any_cast<Network::MessageInfo>(message.mBody);
    messageInfo.senderID = client->getUserID();
    messageInfo.message  = Utility::removeSpaces(messageInfo.message);
    messageInfo.time     = UtilityTime::millisecondsSinceEpoch();

    auto futureResult = _postgreManager->pushRequest(&IMessagesRepository::storeMessage, fmt(messageInfo));

    Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::MessageStoreAnswer;

    auto messageStoringCode = futureResult.get();

    answerForClient.mBody = std::make_any<Utility::StoringMessageCodes>(messageStoringCode);
    client->send(answerForClient);
}

void Server::replyHistoryRequest(const std::shared_ptr<Connection>& client, Message& message) const
{
    auto channelID = std::any_cast<std::uint64_t>(message.mBody);

    auto futureResult = _postgreManager->pushRequest(&IRepliesRepository::getReplyHistory, fmt(channelID));

    Message replyMsg;
    replyMsg.mHeader.mMessageType = Message::MessageType::ReplyHistoryAnswer;

    auto replyHistory = futureResult.get();

    replyMsg.mBody = std::make_any<std::vector<Network::ReplyInfo>>(replyHistory);
    client->send(replyMsg);
}

void Server::replyStoreRequest(const std::shared_ptr<Connection>& client, Message& message) const
{
    auto replyInfo     = std::any_cast<Network::ReplyInfo>(message.mBody);
    replyInfo.senderID = client->getUserID();
    replyInfo.message  = Utility::removeSpaces(replyInfo.message);

    auto futureResult = _postgreManager->pushRequest(&IRepliesRepository::storeReply, fmt(replyInfo));

    Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::ReplyStoreAnswer;

    auto replyStoringCode = futureResult.get();

    answerForClient.mBody = std::make_any<Utility::StoringReplyCodes>(replyStoringCode);
    client->send(answerForClient);
}

void Server::messageDeleteRequest(const std::shared_ptr<Connection>& client, Message& message) const
{
    auto messageInfo     = std::any_cast<Network::MessageInfo>(message.mBody);
    messageInfo.senderID = client->getUserID();

    auto futureResult = _postgreManager->pushRequest(&IMessagesRepository::deleteMessage, fmt(messageInfo));

    Network::Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::MessageDeleteAnswer;

    const auto deletingMessageCode = futureResult.get();
    answerForClient.mBody          = std::make_any<Utility::DeletingMessageCodes>(deletingMessageCode);
    client->send(answerForClient);
}

void Server::messageEditRequest(const std::shared_ptr<Connection>& client, Message& message) const
{
    auto messageInfo     = std::any_cast<Network::MessageInfo>(message.mBody);
    messageInfo.senderID = client->getUserID();

    auto futureResult = _postgreManager->pushRequest(&IMessagesRepository::editMessage, fmt(messageInfo));

    Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::MessageEditAnswer;

    const auto editingMessageCode = futureResult.get();
    answerForClient.mBody         = std::make_any<Utility::EditingMessageCodes>(editingMessageCode);
    client->send(answerForClient);
}

void Server::messageReactionRequest(const std::shared_ptr<Connection>& client, Message& message) const
{
    auto messageInfo     = std::any_cast<Network::MessageInfo>(message.mBody);
    messageInfo.senderID = client->getUserID();

    auto futureResult = _postgreManager->pushRequest(&IMessagesRepository::updateMessageReactions, fmt(messageInfo));

    Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::MessageReactionAnswer;

    const auto reactionMessageCode = futureResult.get();
    answerForClient.mBody          = std::make_any<Utility::ReactionMessageCodes>(reactionMessageCode);
    client->send(answerForClient);
}

void Server::registrationRequest(const std::shared_ptr<Connection>& client, Message& message) const
{
    auto replyInfo = std::any_cast<Network::RegistrationInfo>(message.mBody);

    auto futureResult = _postgreManager->pushRequest(&IRegisterRepository::registerUser, fmt(replyInfo));

    Message messageToClient;
    messageToClient.mHeader.mMessageType = Message::MessageType::RegistrationAnswer;

    auto registrationCode = futureResult.get();

    messageToClient.mBody = std::make_any<Utility::RegistrationCodes>(registrationCode);
    client->send(messageToClient);
}

void Server::loginRequest(const std::shared_ptr<Connection>& client, Message& message) const 
{
    auto loginInfo = std::any_cast<Network::LoginInfo>(message.mBody);

    auto futureResult = _postgreManager->pushRequest(&ILoginRepository::loginUser, fmt(loginInfo));

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

void Server::channelLeaveRequest(const std::shared_ptr<Connection>& client, Message& message) const
{
    Network::ChannelLeaveInfo channelLeaveInfo;

    auto channelName             = std::any_cast<std::string>(message.mBody);
    channelLeaveInfo.creatorID   = client->getUserID();
    channelLeaveInfo.channelName = channelName;

    auto futureResult = _postgreManager->pushRequest(&IChannelsRepository::leaveChannel, fmt(channelLeaveInfo));

    Message messageToClient;
    messageToClient.mHeader.mMessageType = Message::MessageType::ChannelLeaveAnswer;

    auto subscribingChannelCodes = futureResult.get();
    messageToClient.mBody        = std::make_any<Utility::ChannelLeaveCodes>(subscribingChannelCodes);
    client->send(messageToClient);
}

void Server::channelSubscribeRequest(const std::shared_ptr<Connection>& client, Message& message) const
{
    auto channelInfo   = std::any_cast<Network::ChannelSubscriptionInfo>(message.mBody);
    channelInfo.userID = client->getUserID();

    auto futureResult = _postgreManager->pushRequest(&IChannelsRepository::subscribeToChannel, fmt(channelInfo));

    Message messageToClient;
    messageToClient.mHeader.mMessageType = Message::MessageType::ChannelSubscribeAnswer;

    auto subscribingChannelCodes = futureResult.get();

    messageToClient.mBody = std::make_any<Utility::ChannelSubscribingCodes>(subscribingChannelCodes);
    client->send(messageToClient);
}

void Server::channelSubscriptionListRequest(const std::shared_ptr<Connection>& client) const 
{
    const auto userID = client->getUserID();

    auto futureResult = _postgreManager->pushRequest(&IChannelsRepository::getChannelSubscriptionList, fmt(userID));

    Message messageToClient;
    messageToClient.mHeader.mMessageType = Message::MessageType::ChannelSubscriptionListAnswer;

    auto subscribingChannelCodes = futureResult.get();
    messageToClient.mBody        = std::make_any<std::vector<uint64_t>>(subscribingChannelCodes);
    client->send(messageToClient);
}

void Server::channelDeleteRequest(const std::shared_ptr<Connection>& client, Message& message) const
{
    Network::ChannelDeleteInfo channelDeleteInfo;

    auto channelName              = std::any_cast<std::string>(message.mBody);
    channelDeleteInfo.creatorID   = client->getUserID();
    channelDeleteInfo.channelName = channelName;

    auto futureResult = _postgreManager->pushRequest(&IChannelsRepository::deleteChannel, fmt(channelDeleteInfo));

    Message messageToClient;
    messageToClient.mHeader.mMessageType = Message::MessageType::ChannelDeleteAnswer;

    auto deletedChannelCodes = futureResult.get();
    messageToClient.mBody    = std::make_any<Utility::ChannelDeleteCode>(deletedChannelCodes);
    client->send(messageToClient);
}

void Server::channelCreateRequest(const std::shared_ptr<Connection>& client, Message& message) const
{
    Network::ChannelInfo newChannelInfo;

    auto channelName           = std::any_cast<std::string>(message.mBody);
    newChannelInfo.creatorID   = client->getUserID();
    newChannelInfo.channelName = channelName;

    auto futureResult = _postgreManager->pushRequest(&IChannelsRepository::createChannel, fmt(newChannelInfo));

    Message messageToClient;
    messageToClient.mHeader.mMessageType = Message::MessageType::ChannelCreateAnswer;

    auto channelCreateCode = futureResult.get();
    messageToClient.mBody  = std::make_any<Utility::ChannelCreateCodes>(channelCreateCode);
    client->send(messageToClient);
}

void Server::directMessageCreateRequest(const std::shared_ptr<Connection>& client, Message& message) const
{
    auto secondUser = std::any_cast<std::uint64_t>(message.mBody);

    auto futureResult = _postgreManager->pushRequest(&IDirectMessageRepository::addDirectChat, fmt(client->getUserID()), fmt(secondUser));

    Message messageToClient;
    messageToClient.mHeader.mMessageType = Message::MessageType::DirectMessageCreateAnswer;

    messageToClient.mBody = std::make_any<Utility::DirectMessageStatus>(futureResult.get());
    client->send(messageToClient);
}
void Server::defaultRequest() const
{ 
    FileLogger::getInstance().log
    (
        "Unknown command received", 
        LogLevel::ERR
    );
}
}  // namespace Server
