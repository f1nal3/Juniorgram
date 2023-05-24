#include "Server.hpp"

#include <future>

#include "FileLogger.hpp"
#include "Logger/ILogger.hpp"

namespace Server
{
using namespace DataAccess;
using Base::Logger::FileLogger;
using Base::Logger::LogLevel;

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
    _repoManager->handleRequests();

    FileLogger::getInstance().log("[SERVER] Started!", LogLevel::INFO);
}

void Server::stop()
{
    _context.stop();

    _repoManager->stopHandler();

    for (std::thread& thread : _threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }

    _threads.clear();

    FileLogger::getInstance().log("[SERVER] Stopped!", LogLevel::INFO);
}

void Server::waitForClientConnection()
{
    _acceptor->async_accept([this](const std::error_code error, tcp::socket socket) 
        { acceptingClientConnection(error, socket); });
}

void Server::initRepository(RepoManagerUPtr repoManager) { _repoManager.swap(repoManager); }

void Server::initConnection(const uint16_t port) { _acceptor = std::make_unique<tcp::acceptor>(_context, tcp::endpoint(tcp::v4(), port)); }

bool Server::onClientConnect(const std::shared_ptr<Connection>& client)
{
    Message message;
    message.mHeader.mMessageType = Message::MessageType::ServerAccept;
    client->send(message);
    return true;
}

void Server::onClientDisconnect(const std::shared_ptr<Connection>& client)
{
    FileLogger::getInstance().log("Removing client [" + std::to_string(client->getID()) + "]", LogLevel::INFO);
}

void Server::acceptingClientConnection(const std::error_code& error, tcp::socket& socket)
{
    if (!error)
    {
        std::ostringstream outputPoint;
        outputPoint << socket.remote_endpoint();

        FileLogger::getInstance().log("[SERVER] New Connection: " + outputPoint.str(), LogLevel::INFO);

        auto newConnection =
            std::make_shared<Connection>(Connection::OwnerType::SERVER, _context, std::move(socket), _incomingMessagesQueue);

        if (onClientConnect(newConnection))
        {
            _connectionsPointers.emplace_back(std::move(newConnection));
            _connectionsPointers.back()->connectToClient(_idCounter++);

            FileLogger::getInstance().log
            (
                "[" + std::to_string(_connectionsPointers.back()->getID()) 
                + "] Connection Approved", LogLevel::INFO
            );
        }
        else
        {
            FileLogger::getInstance().log("[SERVER] Connection Denied", LogLevel::INFO);
        }
    }
    else
    {
        FileLogger::getInstance().log("[SERVER] New Connection Error: " + error.message(), LogLevel::ERR);
    }

    waitForClientConnection();
}

void Server::onMessage(const std::shared_ptr<Connection>& client, const Message& message)
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

void Server::messageClient(std::shared_ptr<Connection> client, const Message& message) const
{
    if (client != nullptr && client->isConnected())
    {
        client->send(message);
    }
    else
    {
        onClientDisconnect(client);

        client.reset();

        _connectionsPointers.empty();
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
        messagesCount++;
    }
}

void Server::messageAllClients(std::shared_ptr<Connection> exceptionClient, const Message& message) const
{
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
            client->disconnect();

            FileLogger::getInstance().log
            (
                "[Other connections exists: " 
                + std::to_string(client.use_count()) + "]"
            );
        }
    }
}

void Server::checkServerPing(std::shared_ptr<Connection> client, const Message& message) const
{
    std::tm formattedTimestamp = UtilityTime::safe_localtime(message.mHeader.mTimestamp);

    std::ostringstream timeOutput;
    timeOutput << std::put_time(&formattedTimestamp, "%F %T");

    FileLogger::getInstance().log
    (
        "[" + timeOutput.str() 
        + "][" + std::to_string(client->getID())
        + "]: Server Ping", LogLevel::INFO
    );

    client->send(message);
}

void Server::readAllMessage(std::shared_ptr<Connection> client, const Message& message) const
{
    std::tm formattedTimestamp = UtilityTime::safe_localtime(message.mHeader.mTimestamp);

    std::ostringstream timeOutput;
    timeOutput << std::put_time(&formattedTimestamp, "%F %T");

    FileLogger::getInstance().log
    (
        "[" + timeOutput.str() 
        + "][" + std::to_string(client->getID()) 
        + "]: Message All\n", LogLevel::INFO
    );

    Message messageHandler;
    messageHandler.mHeader.mMessageType = Message::MessageType::ServerMessage;    
    messageAllClients(client, messageHandler);
}

void Server::channelListRequest(std::shared_ptr<Connection> client) const
{
    auto futureResult = _repoManager->pushRequest(&IChannelsRepository<DataAccess::PGModelFiller>::getAllChannelsList);

    Message messageHandler;
    messageHandler.mHeader.mMessageType = Message::MessageType::ChannelListRequest;

    auto channelList     = futureResult.get();
    messageHandler.mBody = std::make_any<std::vector<Models::ChannelInfo>>(channelList);

    client->send(messageHandler);
}

void Server::messageHistoryRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    auto channelID = std::any_cast<std::uint64_t>(message.mBody);

    auto futureResult = _repoManager->pushRequest(&IMessagesRepository::getMessageHistory, DataAccess::fmt(channelID));

    Message messageHandler;
    messageHandler.mHeader.mMessageType = Message::MessageType::MessageHistoryAnswer;

    auto messageHistory = futureResult.get();

    messageHandler.mBody = std::make_any<std::vector<Models::MessageInfo>>(messageHistory);
    client->send(messageHandler);
}

void Server::messageStoreRequest(std::shared_ptr<Connection> client, const Message& message) const
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

void Server::replyHistoryRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    auto channelID = std::any_cast<std::uint64_t>(message.mBody);

    auto futureResult = _repoManager->pushRequest(&IRepliesRepository::getReplyHistory, fmt(channelID));

    Message replyMsg;
    replyMsg.mHeader.mMessageType = Message::MessageType::ReplyHistoryAnswer;

    auto replyHistory = futureResult.get();

    replyMsg.mBody = std::make_any<std::vector<Models::ReplyInfo>>(replyHistory);
    client->send(replyMsg);
}

void Server::replyStoreRequest(std::shared_ptr<Connection> client, const Message& message) const
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

void Server::messageDeleteRequest(std::shared_ptr<Connection> client, const Message& message) const
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

void Server::messageEditRequest(std::shared_ptr<Connection> client, const Message& message) const
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

void Server::messageReactionRequest(std::shared_ptr<Connection> client, const Message& message) const
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

void Server::registrationRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    auto replyInfo = std::any_cast<Models::RegistrationInfo>(message.mBody);

    auto futureResult = _repoManager->pushRequest(&IRegisterRepository::registerUser, fmt(replyInfo));

    Message messageToClient;
    messageToClient.mHeader.mMessageType = Message::MessageType::RegistrationAnswer;

    auto registrationCode = futureResult.get();

    messageToClient.mBody = std::make_any<Utility::RegistrationCodes>(registrationCode);
    client->send(messageToClient);
}

void Server::loginRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    auto loginInfo = std::any_cast<Models::LoginInfo>(message.mBody);

    auto futureResult = _repoManager->pushRequest(&ILoginRepository::loginUser, fmt(loginInfo));

    auto userID          = futureResult.get();
    auto loginSuccessful = userID != 0;

    FileLogger::getInstance().log("DEBUG: userID=" + std::to_string(userID), LogLevel::DEBUG);

    Message messageToClient;
    messageToClient.mHeader.mMessageType = Message::MessageType::LoginAnswer;
    messageToClient.mBody                = std::make_any<bool>(loginSuccessful);

    client->send(messageToClient);

    if (loginSuccessful)
    {
        client->setUserID(userID);

        FileLogger::getInstance().log
        (
            "User " + std::to_string(userID) 
            + " logged in.", LogLevel::INFO
        );
    }
}

void Server::channelLeaveRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    Models::ChannelLeaveInfo channelLeaveInfo;

    auto channelName              = std::any_cast<std::string>(message.mBody);
    channelLeaveInfo._creatorID   = client->getUserID();
    channelLeaveInfo._channelName = channelName;

    auto futureResult = _repoManager->pushRequest(&IChannelsRepository<DataAccess::PGModelFiller>::leaveChannel, fmt(channelLeaveInfo));

    Message messageToClient;
    messageToClient.mHeader.mMessageType = Message::MessageType::ChannelLeaveAnswer;

    auto subscribingChannelCodes = futureResult.get();
    messageToClient.mBody        = std::make_any<Utility::ChannelLeaveCodes>(subscribingChannelCodes);
    client->send(messageToClient);
}

void Server::channelSubscribeRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    auto channelInfo    = std::any_cast<Models::ChannelSubscriptionInfo>(message.mBody);
    channelInfo._userID = client->getUserID();

    auto futureResult = _repoManager->pushRequest(&IChannelsRepository<DataAccess::PGModelFiller>::subscribeToChannel, fmt(channelInfo));

    Message messageToClient;
    messageToClient.mHeader.mMessageType = Message::MessageType::ChannelSubscribeAnswer;

    auto subscribingChannelCodes = futureResult.get();

    messageToClient.mBody = std::make_any<Utility::ChannelSubscribingCodes>(subscribingChannelCodes);
    client->send(messageToClient);
}

void Server::channelSubscriptionListRequest(std::shared_ptr<Connection> client) const
{
    const auto userID = client->getUserID();

    auto futureResult = _repoManager->pushRequest(&IChannelsRepository<DataAccess::PGModelFiller>::getChannelSubscriptionList, fmt(userID));

    Message messageToClient;
    messageToClient.mHeader.mMessageType = Message::MessageType::ChannelSubscriptionListAnswer;

    auto subscribingChannelCodes = futureResult.get();
    messageToClient.mBody        = std::make_any<std::vector<uint64_t>>(subscribingChannelCodes);
    client->send(messageToClient);
}

void Server::channelDeleteRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    Models::ChannelDeleteInfo channelDeleteInfo;

    auto channelName               = std::any_cast<std::string>(message.mBody);
    channelDeleteInfo._creatorID   = client->getUserID();
    channelDeleteInfo._channelName = channelName;

    auto futureResult = _repoManager->pushRequest(&IChannelsRepository<DataAccess::PGModelFiller>::deleteChannel, fmt(channelDeleteInfo));

    Message messageToClient;
    messageToClient.mHeader.mMessageType = Message::MessageType::ChannelDeleteAnswer;

    auto deletedChannelCodes = futureResult.get();
    messageToClient.mBody    = std::make_any<Utility::ChannelDeleteCode>(deletedChannelCodes);
    client->send(messageToClient);
}

void Server::channelCreateRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    Models::ChannelInfo newChannelInfo;

    auto channelName            = std::any_cast<std::string>(message.mBody);
    newChannelInfo._creatorID   = client->getUserID();
    newChannelInfo._channelName = channelName;

    auto futureResult = _repoManager->pushRequest(&IChannelsRepository<DataAccess::PGModelFiller>::createChannel, fmt(newChannelInfo));

    Message messageToClient;
    messageToClient.mHeader.mMessageType = Message::MessageType::ChannelCreateAnswer;

    auto channelCreateCode = futureResult.get();
    messageToClient.mBody  = std::make_any<Utility::ChannelCreateCodes>(channelCreateCode);
    client->send(messageToClient);
}

void Server::directMessageCreateRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    auto secondUser = std::any_cast<std::uint64_t>(message.mBody);

    auto futureResult = _repoManager->pushRequest(&IDirectMessageRepository::addDirectChat, fmt(client->getUserID()), fmt(secondUser));

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
