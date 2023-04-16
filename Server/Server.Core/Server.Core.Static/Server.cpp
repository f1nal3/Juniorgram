#include "Server.hpp"

#include <future>

#include "FileLogger.hpp"
#include "Logger/ILogger.hpp"
#include <Models/Models.hpp>

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

void Server::checkMessageResult(std::optional<MessageResult> result)
{
    if (result != MessageResult::Success)
    {
        _messageResponce.emplace_back(result.value());
    }
    _messageResponce.emplace_back(MessageResult::Success);
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
    //there should connection verifing
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

void Server::onMessage(const std::shared_ptr<Connection>& client, const Message& message) const
{
    std::optional<Network::MessageResult> Result;

    switch (message.mHeader.mMessageType)
    {
        case Message::MessageType::ServerPing:
        {
            Result = checkServerPing(client, message);
            break;
        }
        case Message::MessageType::MessageAll:
        {
            Result = readAllMessage(client, message);
            break;
        }
        case Message::MessageType::ChannelListRequest:
        {
            Result = channelListRequest(client);
            break;
        }
        case Message::MessageType::MessageHistoryRequest:
        {
            Result = messageHistoryRequest(client, message);
            break;
        }
        case Message::MessageType::MessageStoreRequest:
        {
            Result = messageStoreRequest(client, message);
            break;
        }
        case Message::MessageType::ReplyHistoryRequest:
        {
            Result = replyHistoryRequest(client, message);
            break;
        }
        case Message::MessageType::ReplyStoreRequest:
        {
            Result = replyStoreRequest(client, message);
            break;
        }
        case Message::MessageType::MessageDeleteRequest:
        {
            Result = messageDeleteRequest(client, message);
            break;
        }
        case Message::MessageType::MessageEditRequest:
        {
            Result = messageEditRequest(client, message);
            break;
        }
        case Message::MessageType::MessageReactionRequest:
        {
            Result = messageReactionRequest(client, message);
            break;
        }
        case Message::MessageType::RegistrationRequest:
        {
            Result = registrationRequest(client, message);
            break;
        }
        case Message::MessageType::LoginRequest:
        {
            Result = loginRequest(client, message);
            break;
        }
        case Message::MessageType::ChannelLeaveRequest:
        {
            Result = channelLeaveRequest(client, message);
            break;
        }
        case Message::MessageType::ChannelSubscribeRequest:
        {
            Result = channelSubscribeRequest(client, message);
            break;
        }
        case Message::MessageType::ChannelSubscriptionListRequest:
        {
            Result = channelSubscriptionListRequest(client);
            break;
        }
        case Message::MessageType::ChannelDeleteRequest:
        {
            Result = channelDeleteRequest(client, message);
            break;
        }
        case Message::MessageType::ChannelCreateRequest:
        {
            Result = channelCreateRequest(client, message);
            break;
        }
        case Message::MessageType::DirectMessageCreateRequest:
        {
            Result = directMessageCreateRequest(client, message);
            break;
        }

        case Message::MessageType::RequestOnConnection:
        {
            directRequestOnConnection(client);
            break;
        }

        default:
        {
            Result = defaultRequest();
            break;
        }
    }
}

std::vector<MessageResult> Server::getMessageResult() const { return _messageResponce; }

std::optional<MessageResult> Server::messageClient(std::shared_ptr<Connection> client, const Message& message) const
{
    if (client != nullptr && client->isConnected())
    {
        client->send(message);

        return MessageResult::Success;
    }
    else
    {
        onClientDisconnect(client);
        client.reset();

        return MessageResult::InvalidBody;
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

std::optional<MessageResult> Server::messageAllClients(std::shared_ptr<Connection> exceptionClient, const Message& message) const
{
    for (auto& client : _connectionsPointers)
    {
        if (client != nullptr && client->isConnected())
        {
            if (client != exceptionClient)
            {
                client->send(message);

                return MessageResult::Success;
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

            return MessageResult::InvalidBody;
        }
    }
}

std::optional<Network::MessageResult> Server::checkServerPing(std::shared_ptr<Connection> client, const Message& message) const
{
    std::tm formattedTimestamp = UtilityTime::safe_localtime(message.mHeader.mTimestamp);

    std::ostringstream timeOutput;
    timeOutput << std::put_time(&formattedTimestamp, "%F %T");

    if (message.mHeader.mMessageType == Message::MessageType::ServerPing)
    {
        FileLogger::getInstance().log
        (
            "[" + timeOutput.str() 
            + "][" + std::to_string(client->getID())
            + "]: Server Ping", LogLevel::INFO
        );

        client->send(message);

        return MessageResult::Success;
    }
    return MessageResult::InvalidBody;
}

std::optional<Network::MessageResult> Server::readAllMessage(std::shared_ptr<Connection> client, const Message& message) const
{
    std::tm formattedTimestamp = UtilityTime::safe_localtime(message.mHeader.mTimestamp);

    std::ostringstream timeOutput;
    timeOutput << std::put_time(&formattedTimestamp, "%F %T");

    if (message.mHeader.mMessageType == Message::MessageType::MessageAll)
    {
        FileLogger::getInstance().log
        (
            "[" + timeOutput.str() 
            + "][" + std::to_string(client->getID()) 
            + "]: Message All\n", LogLevel::INFO
        );

        Message answerForClient;
        answerForClient.mHeader.mMessageType = Message::MessageType::ServerMessage;    

        messageAllClients(client, answerForClient);

        return MessageResult::Success;
    }
    return MessageResult::InvalidBody;
}

std::optional<Network::MessageResult> Server::channelListRequest(std::shared_ptr<Connection> client) const
{
    auto futureResult = _repoManager->pushRequest(&IChannelsRepository::getAllChannelsList);

    Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::ChannelListRequest;

    auto channelList                    = futureResult.get();
    answerForClient.mBody = std::make_any<std::vector<Models::ChannelInfo>>(channelList);

    if (answerForClient.mHeader.mMessageType == Message::MessageType::ChannelListRequest)
    {
        client->send(answerForClient);

        return MessageResult::Success;
    }
    return MessageResult::InvalidBody;
}

std::optional<Network::MessageResult> Server::messageHistoryRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    auto channelID = std::any_cast<std::uint64_t>(message.mBody);

    auto futureResult = _repoManager->pushRequest(&IMessagesRepository::getMessageHistory, DataAccess::fmt(channelID));

    Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::MessageHistoryAnswer;

    if (message.mHeader.mMessageType == Message::MessageType::MessageHistoryRequest)
    {
        auto messageHistory = futureResult.get();
        answerForClient.mBody = std::make_any<std::vector<Models::MessageInfo>>(messageHistory);

        client->send(answerForClient);

        return MessageResult::Success;
    }
    return MessageResult::InvalidBody;
}

std::optional<Network::MessageResult> Server::messageStoreRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    auto messageInfo      = std::any_cast<Models::MessageInfo>(message.mBody);
    messageInfo._senderID = client->getUserID();
    messageInfo._message  = Utility::removeSpaces(messageInfo._message);
    messageInfo._time     = UtilityTime::millisecondsSinceEpoch();

    auto futureResult = _repoManager->pushRequest(&IMessagesRepository::storeMessage, fmt(messageInfo));

    Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::MessageStoreAnswer;

    if (message.mHeader.mMessageType == Message::MessageType::MessageStoreRequest)
    {
        auto messageStoringCode = futureResult.get();
        answerForClient.mBody = std::make_any<Utility::StoringMessageCodes>(messageStoringCode);

        client->send(answerForClient);

        return MessageResult::Success;
    }
    return MessageResult::InvalidBody;
}

std::optional<Network::MessageResult> Server::replyHistoryRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    auto channelID = std::any_cast<std::uint64_t>(message.mBody);

    auto futureResult = _repoManager->pushRequest(&IRepliesRepository::getReplyHistory, fmt(channelID));

    Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::ReplyHistoryAnswer;

    if (message.mHeader.mMessageType == Message::MessageType::ReplyHistoryRequest)
    {
        auto replyHistory = futureResult.get();
        answerForClient.mBody = std::make_any<std::vector<Models::ReplyInfo>>(replyHistory);

        client->send(answerForClient);

        return MessageResult::Success;
    }
    return MessageResult::InvalidBody;
}

std::optional<Network::MessageResult> Server::replyStoreRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    auto replyInfo      = std::any_cast<Models::ReplyInfo>(message.mBody);
    replyInfo._senderID = client->getUserID();
    replyInfo._message  = Utility::removeSpaces(replyInfo._message);

    auto futureResult = _repoManager->pushRequest(&IRepliesRepository::storeReply, fmt(replyInfo));

    Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::ReplyStoreAnswer;

    if (message.mHeader.mMessageType == Message::MessageType::ReplyStoreRequest)
    {
        auto replyStoringCode = futureResult.get();
        answerForClient.mBody = std::make_any<Utility::StoringReplyCodes>(replyStoringCode);

        client->send(answerForClient);

        return MessageResult::Success;
    }
    return MessageResult::InvalidBody;
}

std::optional<Network::MessageResult> Server::messageDeleteRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    auto messageInfo      = std::any_cast<Models::MessageInfo>(message.mBody);
    messageInfo._senderID = client->getUserID();

    auto futureResult = _repoManager->pushRequest(&IMessagesRepository::deleteMessage, fmt(messageInfo));

    Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::MessageDeleteAnswer;

    if (message.mHeader.mMessageType == Message::MessageType::MessageDeleteRequest)
    {
        const auto deletingMessageCode = futureResult.get();
        answerForClient.mBody          = std::make_any<Utility::DeletingMessageCodes>(deletingMessageCode);

        client->send(answerForClient);

        return MessageResult::Success;
    }
    return MessageResult::InvalidBody;
}

std::optional<Network::MessageResult> Server::messageEditRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    auto messageInfo      = std::any_cast<Models::MessageInfo>(message.mBody);
    messageInfo._senderID = client->getUserID();

    auto futureResult = _repoManager->pushRequest(&IMessagesRepository::editMessage, fmt(messageInfo));

    Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::MessageEditAnswer;

    if (message.mHeader.mMessageType == Message::MessageType::MessageEditRequest)
    {
        const auto editingMessageCode = futureResult.get();
        answerForClient.mBody         = std::make_any<Utility::EditingMessageCodes>(editingMessageCode);

        client->send(answerForClient);

        return MessageResult::Success;
    }
    return MessageResult::InvalidBody;
}

std::optional<Network::MessageResult> Server::messageReactionRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    auto messageInfo      = std::any_cast<Models::MessageInfo>(message.mBody);
    messageInfo._senderID = client->getUserID();

    auto futureResult = _repoManager->pushRequest(&IMessagesRepository::updateMessageReactions, fmt(messageInfo));

    Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::MessageReactionAnswer;

    if (message.mHeader.mMessageType == Message::MessageType::MessageReactionRequest)
    {
        const auto reactionMessageCode = futureResult.get();
        answerForClient.mBody          = std::make_any<Utility::ReactionMessageCodes>(reactionMessageCode);

        client->send(answerForClient);

        return MessageResult::Success;
    }
    return MessageResult::InvalidBody;
}

std::optional<Network::MessageResult> Server::registrationRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    auto replyInfo = std::any_cast<Models::RegistrationInfo>(message.mBody);

    auto futureResult = _repoManager->pushRequest(&IRegisterRepository::registerUser, fmt(replyInfo));

    Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::RegistrationAnswer;

    if (message.mHeader.mMessageType == Message::MessageType::RegistrationRequest)
    {
        auto registrationCode = futureResult.get();
        answerForClient.mBody = std::make_any<Utility::RegistrationCodes>(registrationCode);

        client->send(answerForClient);

        return MessageResult::Success;
    }
    return MessageResult::InvalidBody;
}

std::optional<Network::MessageResult> Server::loginRequest(std::shared_ptr<Connection> client, const Message& message) const
{

    auto loginInfo = std::any_cast<Models::LoginInfo>(message.mBody);

    auto futureResult = _repoManager->pushRequest(&ILoginRepository::loginUser, fmt(loginInfo));

    if (message.mHeader.mMessageType == Message::MessageType::LoginRequest)
    {
        auto userID          = futureResult.get();
        auto loginSuccessful = userID != 0;

        FileLogger::getInstance().log("DEBUG: userID=" + std::to_string(userID), LogLevel::DEBUG);

        Message answerForClient;
        answerForClient.mHeader.mMessageType = Message::MessageType::LoginAnswer;
        answerForClient.mBody                = std::make_any<bool>(loginSuccessful);

        client->send(answerForClient);

        if (loginSuccessful)
        {
            client->setUserID(userID);

            FileLogger::getInstance().log("User " + std::to_string(userID) + " logged in.", LogLevel::INFO);
        }

        return MessageResult::Success;
    }
    return MessageResult::InvalidBody;
}

std::optional<Network::MessageResult> Server::channelLeaveRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    Models::ChannelLeaveInfo channelLeaveInfo;

    auto channelName              = std::any_cast<std::string>(message.mBody);
    channelLeaveInfo._creatorID   = client->getUserID();
    channelLeaveInfo._channelName = channelName;

    auto futureResult = _repoManager->pushRequest(&IChannelsRepository::leaveChannel, fmt(channelLeaveInfo));
   
    Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::ChannelLeaveAnswer;

    if (message.mHeader.mMessageType == Message::MessageType::ChannelLeaveRequest)
    {
        auto subscribingChannelCodes = futureResult.get();
        answerForClient.mBody        = std::make_any<Utility::ChannelLeaveCodes>(subscribingChannelCodes);

        client->send(answerForClient);

        return MessageResult::Success;
    }
    return MessageResult::InvalidBody;
}

std::optional<Network::MessageResult> Server::channelSubscribeRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    auto channelInfo    = std::any_cast<Models::ChannelSubscriptionInfo>(message.mBody);
    channelInfo._userID = client->getUserID();

    auto futureResult = _repoManager->pushRequest(&IChannelsRepository::subscribeToChannel, fmt(channelInfo));

    Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::ChannelSubscribeAnswer;

    if (message.mHeader.mMessageType == Message::MessageType::ChannelSubscribeRequest)
    {
        auto subscribingChannelCodes = futureResult.get();

        answerForClient.mBody = std::make_any<Utility::ChannelSubscribingCodes>(subscribingChannelCodes);

        client->send(answerForClient);

        return MessageResult::Success;
    }
    return MessageResult::InvalidBody;
}

std::optional<Network::MessageResult> Server::channelSubscriptionListRequest(std::shared_ptr<Connection> client) const
{
    const auto userID = client->getUserID();

    auto futureResult = _repoManager->pushRequest(&IChannelsRepository::getChannelSubscriptionList, fmt(userID));

    Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::ChannelSubscriptionListAnswer;

    auto subscribingChannelCodes = futureResult.get();
    answerForClient.mBody        = std::make_any<std::vector<uint64_t>>(subscribingChannelCodes);

    if (answerForClient.mHeader.mMessageType == Message::MessageType::ChannelSubscriptionListAnswer)
    {
        client->send(answerForClient);

        return MessageResult::Success;
    }
    return MessageResult::InvalidBody;
}

std::optional<Network::MessageResult> Server::channelDeleteRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    Models::ChannelDeleteInfo channelDeleteInfo;

    auto channelName               = std::any_cast<std::string>(message.mBody);
    channelDeleteInfo._creatorID   = client->getUserID();
    channelDeleteInfo._channelName = channelName;

    auto futureResult = _repoManager->pushRequest(&IChannelsRepository::deleteChannel, fmt(channelDeleteInfo));

    Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::ChannelDeleteAnswer;

    if (message.mHeader.mMessageType == Message::MessageType::ChannelDeleteRequest)
    {
        auto deletedChannelCodes = futureResult.get();
        answerForClient.mBody    = std::make_any<Utility::ChannelDeleteCode>(deletedChannelCodes);

        client->send(answerForClient);

        return MessageResult::Success;
    }
    return MessageResult::InvalidBody;
}

std::optional<Network::MessageResult> Server::channelCreateRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    using Models::V2::ChannelData;

    Models::V2::Channel<> newChannelInfo({
                                   {ChannelData::CHANNEL_NAME, std::any_cast<std::string>(message.mBody)},
                                   {ChannelData::CREATOR_ID, std::to_string(client->getUserID())}});

    auto futureResult = _repoManager->pushRequest(&IChannelsRepository::newCreateChannel, fmt(newChannelInfo));

    Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::ChannelCreateAnswer;

    if (message.mHeader.mMessageType == Message::MessageType::ChannelCreateRequest)
    {
        auto channelCreateCode = futureResult.get();
        answerForClient.mBody  = std::make_any<Utility::ChannelCreateCodes>(channelCreateCode);

        client->send(answerForClient);

        return MessageResult::Success;
    }
    return MessageResult::InvalidBody;
}

std::optional<Network::MessageResult> Server::directMessageCreateRequest(std::shared_ptr<Connection> client, const Message& message) const
{
    auto secondUser = std::any_cast<std::uint64_t>(message.mBody);

    auto futureResult = _repoManager->pushRequest(&IDirectMessageRepository::addDirectChat, fmt(client->getUserID()), fmt(secondUser));

    Message answerForClient;
    answerForClient.mHeader.mMessageType = Message::MessageType::DirectMessageCreateAnswer;

    if (message.mHeader.mMessageType == Message::MessageType::DirectMessageCreateRequest)
    {
        answerForClient.mBody = std::make_any<Utility::DirectMessageStatus>(futureResult.get());

        client->send(answerForClient);

        return MessageResult::Success;
    }
    return MessageResult::InvalidBody;
}

std::optional<MessageResult> Server::directRequestOnConnection(std::shared_ptr<Network::Connection> client) const
{
    Message messageToClient;
    messageToClient.mHeader.mMessageType = Message::MessageType::RequestOnConnectionAnswer;

    Models::ConnectionInfo connectionInfo;
    connectionInfo._connectionID   = client->getID();
    connectionInfo._publicServerKey = _rsaKeyManager->getPublicServerKeyStr();

    messageToClient.mBody = std::make_any<Models::ConnectionInfo>(connectionInfo);
    client->send(messageToClient);
}

void Server::defaultRequest() const 
{
    FileLogger::getInstance().log
    (
        "Unknown command received", 
        LogLevel::ERR
    );
    return MessageResult::InvalidBody;
}
}  // namespace Server
