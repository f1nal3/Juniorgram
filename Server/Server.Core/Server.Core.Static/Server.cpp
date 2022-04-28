#include <future>

#include "Server.hpp"
#include "DataAccess.Postgre/PostgreRepositoryManager.hpp"

using Network::Connection;
using Network::Message;
using Network::SafeQueue;
using DataAccess::PostgreAdapter;
using DataAccess::PostgreRepositoryManager;

namespace Server
{

bool Server::onClientConnect(const std::shared_ptr<Connection>& client)
{
    Message messageHandler;
    messageHandler._header._messageType = Message::MessageType::ServerAccept;

    client->send(messageHandler);
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
    const auto currentTime     = std::chrono::system_clock::now();
    message._header._timestamp = currentTime;

    switch (message._header._messageType)
    {
        case Network::Message::MessageType::ServerPing:
        {
            checkServerPing(client,message);
        }
        break;

        case Network::Message::MessageType::MessageAll:
        {
            readAllMessage(client, message);
        }
        break;

        case Network::Message::MessageType::ChannelListRequest:
        {
            channelListRequest(client, message);
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
            ChannelSubscriptionListRequest(client, message);
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
Server::Server(const uint16_t& port)
    :_acceptor(_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
     _IDCounter(10000),
     _criticalQueueSize(100),
     _newThreadsCount(std::thread::hardware_concurrency()),
     _postgreManager
     (std::make_unique<PostgreRepositoryManager>(PostgreAdapter::getInstance<PostgreAdapter>()))
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

    Base::Logger::FileLogger::getInstance().log
    (
        "[SERVER] Started!", Base::Logger::LogLevel::INFO
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

    Base::Logger::FileLogger::getInstance().log
    (
        "[SERVER] Stopped!", Base::Logger::LogLevel::INFO
    );
}

void Server::waitForClientConnection()
{
    _acceptor.async_accept([this](std::error_code error, asio::ip::tcp::socket socket) { acceptingClientConnection(error,socket); });
}

void Server::acceptingClientConnection(std::error_code& error, asio::ip::tcp::socket& socket)
{
    if (!error)
    {
        std::ostringstream outputPoint;
        outputPoint << socket.remote_endpoint();

        Base::Logger::FileLogger::getInstance().log
        (
            "[SERVER] New Connection: " + outputPoint.str(), Base::Logger::LogLevel::INFO
        );

        auto newConnection = std::make_shared<Connection>(Connection::OwnerType::SERVER, _context, std::move(socket), _incomingMessagesQueue);

        if (onClientConnect(newConnection))
        {
            _connectionsPointers.push_back(std::move(newConnection));
            _connectionsPointers.back()->connectToClient(++_IDCounter);

            Base::Logger::FileLogger::getInstance().log
            (
                "[" + std::to_string(_connectionsPointers.back()->getID()) 
                + "] Connection Approved", Base::Logger::LogLevel::INFO
            );
        }
        else
        {
            Base::Logger::FileLogger::getInstance().log
            (
                "[-----] Connection Denied", Base::Logger::LogLevel::INFO
            );
        }
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[SERVER] New Connection Error: " + error.message(), Base::Logger::LogLevel::ERR
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

        onMessage(messageInfo._remote, messageInfo);
        ++messagesCount;
    }
}

void Server::checkServerPing(const std::shared_ptr<Connection>& client, Message& message)
{
    using namespace DataAccess;

    std::tm formattedTimestamp = Utility::safe_localtime(std::chrono::system_clock::to_time_t(message._header._timestamp));

    std::ostringstream timeOutput;
    timeOutput << std::put_time(&formattedTimestamp, "%F %T");

    Base::Logger::FileLogger::getInstance().log
    (
        "[" + timeOutput.str() + "][" + std::to_string(client->getID()) + "]: Server Ping",
                                                Base::Logger::LogLevel::INFO
    );

    client->send(message);
}

void Server::readAllMessage(const std::shared_ptr<Connection>& client, Message& message)
{
    using namespace DataAccess;

    std::tm formattedTimestamp = Utility::safe_localtime(std::chrono::system_clock::to_time_t(message._header._timestamp));

    std::ostringstream timeOutput;
    timeOutput << std::put_time(&formattedTimestamp, "%F %T");

    Base::Logger::FileLogger::getInstance().log
    (
        "[" + timeOutput.str() + "][" + std::to_string(client->getID()) + "]: Message All\n",
                                                Base::Logger::LogLevel::INFO
    );
   
    Message messageHandler;
    messageHandler._header._messageType = Message::MessageType::ServerMessage;
    // msg << client->getID();

    messageAllClients(messageHandler, client);
}

void Server::channelListRequest(const std::shared_ptr<Connection>& client, Message& message)
{
    using namespace DataAccess;

    auto futureResult = _postgreManager->pushRequest(&IChannelsRepository::getAllChannelsList);

    Message messageHandler;
    messageHandler._header._messageType = Message::MessageType::ChannelListRequest;

    auto channelList     = futureResult.get();
    messageHandler._body = std::make_any<std::vector<Network::ChannelInfo>>(channelList);

    client->send(messageHandler);
}

void Server::messageHistoryRequest(const std::shared_ptr<Connection>& client, Message& message)
{
    using namespace DataAccess;

    auto channelID = std::any_cast<std::uint64_t>(message._body);

    auto futureResult = _postgreManager->pushRequest(&IMessagesRepository::getMessageHistory, DataAccess::fmt(channelID));

    Message messageHandler;
    messageHandler._header._messageType = Message::MessageType::MessageHistoryAnswer;

    auto messageHistory = futureResult.get();

    messageHandler._body = std::make_any<std::vector<Network::MessageInfo>>(messageHistory);
    client->send(messageHandler);
}

void Server::messageStoreRequest(const std::shared_ptr<Connection>& client, Message& message)
{
    using namespace DataAccess;

    auto messageInfo      = std::any_cast<Network::MessageInfo>(message._body);
    messageInfo._senderID = client->getUserID();
    messageInfo._message  = Utility::removeSpaces(messageInfo._message);
    messageInfo._time     = Utility::millisecondsSinceEpoch();

    auto futureResult = _postgreManager->pushRequest(&IMessagesRepository::storeMessage, fmt(messageInfo));

    Message answerForClient;
    answerForClient._header._messageType = Message::MessageType::MessageStoreAnswer;

    auto messageStoringCode = futureResult.get();

    answerForClient._body = std::make_any<Utility::StoringMessageCodes>(messageStoringCode);
    client->send(answerForClient);
}

void Server::replyHistoryRequest(const std::shared_ptr<Connection>& client, Message& message)
{
    using namespace DataAccess;

    auto channelID = std::any_cast<std::uint64_t>(message._body);

    auto futureResult = _postgreManager->pushRequest(&IRepliesRepository::getReplyHistory, fmt(channelID));

    Message replyMsg;
    replyMsg._header._messageType = Message::MessageType::ReplyHistoryAnswer;

    auto replyHistory = futureResult.get();

    replyMsg._body = std::make_any<std::vector<Network::ReplyInfo>>(replyHistory);
    client->send(replyMsg);
}

void Server::replyStoreRequest(const std::shared_ptr<Connection>& client, Message& message) 
{
    using namespace DataAccess;

    auto replyInfo      = std::any_cast<Network::ReplyInfo>(message._body);
    replyInfo._senderID = client->getUserID();
    replyInfo._message  = Utility::removeSpaces(replyInfo._message);

    auto futureResult = _postgreManager->pushRequest(&IRepliesRepository::storeReply, fmt(replyInfo));

    Message answerForClient;
    answerForClient._header._messageType = Message::MessageType::ReplyStoreAnswer;

    auto replyStoringCode = futureResult.get();

    answerForClient._body = std::make_any<Utility::StoringReplyCodes>(replyStoringCode);
    client->send(answerForClient);
}

void Server::messageDeleteRequest(const std::shared_ptr<Connection>& client, Message& message)
{
    using namespace DataAccess;

    auto messageInfo      = std::any_cast<Network::MessageInfo>(message._body);
    messageInfo._senderID = client->getUserID();

    auto futureResult = _postgreManager->pushRequest(&IMessagesRepository::deleteMessage, fmt(messageInfo));

    Network::Message answerForClient;
    answerForClient._header._messageType = Message::MessageType::MessageDeleteAnswer;

    const auto deletingMessageCode = futureResult.get();
    answerForClient._body          = std::make_any<Utility::DeletingMessageCodes>(deletingMessageCode);
    client->send(answerForClient);
}

void Server::messageEditRequest(const std::shared_ptr<Connection>& client, Message& message)
{
    using namespace DataAccess;

    auto messageInfo      = std::any_cast<Network::MessageInfo>(message._body);
    messageInfo._senderID = client->getUserID();

    auto futureResult = _postgreManager->pushRequest(&IMessagesRepository::editMessage, fmt(messageInfo));

    Message answerForClient;
    answerForClient._header._messageType = Message::MessageType::MessageEditAnswer;

    const auto editingMessageCode = futureResult.get();
    answerForClient._body         = std::make_any<Utility::EditingMessageCodes>(editingMessageCode);
    client->send(answerForClient);
}

void Server::messageReactionRequest(const std::shared_ptr<Connection>& client, Message& message) 
{
    using namespace DataAccess;

    auto messageInfo      = std::any_cast<Network::MessageInfo>(message._body);
    messageInfo._senderID = client->getUserID();

    auto futureResult = _postgreManager->pushRequest(&IMessagesRepository::updateMessageReactions, fmt(messageInfo));

    Message answerForClient;
    answerForClient._header._messageType = Message::MessageType::MessageReactionAnswer;

    const auto reactionMessageCode = futureResult.get();
    answerForClient._body          = std::make_any<Utility::ReactionMessageCodes>(reactionMessageCode);
    client->send(answerForClient);
}

void Server::registrationRequest(const std::shared_ptr<Connection>& client, Message& message)
{
    using namespace DataAccess;

    auto replyInfo = std::any_cast<Network::RegistrationInfo>(message._body);

    auto futureResult = _postgreManager->pushRequest(&IRegisterRepository::registerUser, fmt(replyInfo));

    Message messageToClient;
    messageToClient._header._messageType = Message::MessageType::RegistrationAnswer;

    auto registrationCode = futureResult.get();

    messageToClient._body = std::make_any<Utility::RegistrationCodes>(registrationCode);
    client->send(messageToClient);
}

void Server::loginRequest(const std::shared_ptr<Connection>& client, Message& message) 
{
    using namespace DataAccess;

    auto loginInfo = std::any_cast<Network::LoginInfo>(message._body);

    auto futureResult = _postgreManager->pushRequest(&ILoginRepository::loginUser, fmt(loginInfo));

    auto userID          = futureResult.get();
    auto loginSuccessful = userID != 0;

    Base::Logger::FileLogger::getInstance().log
    (
        "DEBUG: userID=" + std::to_string(userID), Base::Logger::LogLevel::DEBUG
    );

    Message messageToClient;
    messageToClient._header._messageType = Message::MessageType::LoginAnswer;
    messageToClient._body                = std::make_any<bool>(loginSuccessful);

    client->send(messageToClient);

    if (loginSuccessful)
    {
        client->setUserID(userID);

        Base::Logger::FileLogger::getInstance().log
        (
            "User " + std::to_string(userID) + " logged in.", Base::Logger::LogLevel::INFO
        );
    }
}

void Server::channelLeaveRequest(const std::shared_ptr<Connection>& client, Message& message)
{
    using namespace DataAccess;

    Network::ChannelLeaveInfo channelLeaveInfo;

    auto channelName              = std::any_cast<std::string>(message._body);
    channelLeaveInfo._creatorID   = client->getUserID();
    channelLeaveInfo._channelName = channelName;

    auto futureResult = _postgreManager->pushRequest(&IChannelsRepository::leaveChannel, fmt(channelLeaveInfo));

    Message messageToClient;
    messageToClient._header._messageType = Message::MessageType::ChannelLeaveAnswer;

    auto subscribingChannelCodes = futureResult.get();
    messageToClient._body        = std::make_any<Utility::ChannelLeaveCodes>(subscribingChannelCodes);
    client->send(messageToClient);
}

void Server::channelSubscribeRequest(const std::shared_ptr<Connection>& client, Message& message)
{
    using namespace DataAccess;

    auto channelInfo    = std::any_cast<Network::ChannelSubscriptionInfo>(message._body);
    channelInfo._userID = client->getUserID();

    auto futureResult = _postgreManager->pushRequest(&IChannelsRepository::subscribeToChannel, fmt(channelInfo));

    Message messageToClient;
    messageToClient._header._messageType = Message::MessageType::ChannelSubscribeAnswer;

    auto subscribingChannelCodes = futureResult.get();

    messageToClient._body = std::make_any<Utility::ChannelSubscribingCodes>(subscribingChannelCodes);
    client->send(messageToClient);
}

void Server::ChannelSubscriptionListRequest(const std::shared_ptr<Connection>& client, Message& message) 
{
    using namespace DataAccess;

    const auto userID = client->getUserID();

    auto futureResult = _postgreManager->pushRequest(&IChannelsRepository::getChannelSubscriptionList, fmt(userID));

    Message messageToClient;
    messageToClient._header._messageType = Message::MessageType::ChannelSubscriptionListAnswer;

    auto subscribingChannelCodes = futureResult.get();
    messageToClient._body        = std::make_any<std::vector<uint64_t>>(subscribingChannelCodes);
    client->send(messageToClient);
}

void Server::channelDeleteRequest(const std::shared_ptr<Connection>& client, Message& message) 
{
    using namespace DataAccess;

    Network::ChannelDeleteInfo channelDeleteInfo;

    auto channelName               = std::any_cast<std::string>(message._body);
    channelDeleteInfo._creatorID   = client->getUserID();
    channelDeleteInfo._channelName = channelName;

    auto futureResult = _postgreManager->pushRequest(&IChannelsRepository::deleteChannel, fmt(channelDeleteInfo));

    Message messageToClient;
    messageToClient._header._messageType = Message::MessageType::ChannelDeleteAnswer;

    auto deletedChannelCodes = futureResult.get();
    messageToClient._body    = std::make_any<Utility::ChannelDeleteCode>(deletedChannelCodes);
    client->send(messageToClient);
}

void Server::channelCreateRequest(const std::shared_ptr<Connection>& client, Message& message)
{
    using namespace DataAccess;

    Network::ChannelInfo newChannelInfo;

    auto channelName            = std::any_cast<std::string>(message._body);
    newChannelInfo._creatorID   = client->getUserID();
    newChannelInfo._channelName = channelName;

    auto futureResult = _postgreManager->pushRequest(&IChannelsRepository::createChannel, fmt(newChannelInfo));

    Message messageToClient;
    messageToClient._header._messageType = Message::MessageType::ChannelCreateAnswer;

    auto channelCreateCode = futureResult.get();
    messageToClient._body  = std::make_any<Utility::ChannelCreateCodes>(channelCreateCode);
    client->send(messageToClient);
}

void Server::directMessageCreateRequest(const std::shared_ptr<Connection>& client, Message& message) 
{
    using namespace DataAccess;

    auto secondUser = std::any_cast<std::uint64_t>(message._body);

    auto futureResult = _postgreManager->pushRequest(&IDirectMessageRepository::addDirectChat, fmt(client->getUserID()), fmt(secondUser));

    Message messageToClient;
    messageToClient._header._messageType = Message::MessageType::DirectMessageCreateAnswer;

    messageToClient._body = std::make_any<Utility::DirectMessageStatus>(futureResult.get());
    client->send(messageToClient);
}
void Server::defaultRequest()
{ 
    Base::Logger::FileLogger::getInstance().log
    (
        "Unknown command received", Base::Logger::LogLevel::ERR
    );
}
}  /// namespace Server
