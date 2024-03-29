#include "Client.hpp"
#include "ServerInfo.hpp"

#include <limits>
#include <Crypto.Static/Cryptography.hpp>

namespace Network
{
using MessageType = Network::Message::MessageType;
using UtilityTime::RTC;
using UtilityTime::timestamp_t;

Client::~Client() noexcept { disconnectFromServer(); }

bool Client::connectToServer(const std::string_view& host, const uint16_t port)
{
    if (host != ServerInfo::Address::remote && host != ServerInfo::Address::local)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "Bad server address", 
            Base::Logger::LogLevel::ERR
        );

        return false;
    }
    if (port != ServerInfo::Port::test && port != ServerInfo::Port::production)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "Bad port value",
            Base::Logger::LogLevel::ERR
        );

        return false;
    }

    asio::ip::tcp::resolver resolver(_context);

    using OwnerType = Connection::OwnerType;
    _connection     = std::make_unique<Connection>(OwnerType::CLIENT, _context, 
        asio::ip::tcp::socket(_context), _incomingMessagesQueue);

    try
    {
        /// Auto = asio::ip::tcp::resolver::results_type
        auto endpoints = resolver.resolve(host, std::to_string(port));
        _connection->connectToServer(endpoints);
        _contextThread = std::thread([=]() {
            while (_context.run_one())
            {
                loop();
            }
            _serverAccept = false;
            onDisconnect();
        });
    }
    catch (const std::exception& exception)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "Client Exception: " + std::string(exception.what()), 
            Base::Logger::LogLevel::ERR
        );
        return false;
    }
    return true;
}

void Client::disconnectFromServer()
{
    if (isConnected())
    {
        _connection->disconnect();
    }

    _context.stop();

    if (_contextThread.get_id() != std::this_thread::get_id() && _contextThread.joinable())
    {
        _contextThread.join();
    }

    _connection.reset();
}

bool Client::isConnected() const
{
    if (_connection != nullptr)
    {
        return _connection->isConnected() && _serverAccept;
    }
    return false;
}

Utility::SafeQueue<Message>& Client::incoming()
{
    return _incomingMessagesQueue;
}

void Client::send(const Message& message) const
{
    if (isConnected())
    {
        _connection->send(message);
    }
    else
    {
        onMessageSendFailed(message);
    }
}
void Client::pingServer() const
{
    Network::Message message;
    message.mHeader.mMessageType = MessageType::ServerPing;
    message.mHeader.mTimestamp = RTC::to_time_t(RTC::now());

    send(message);
}

void Client::askForChannelList() const
{
    Network::Message message;
    message.mHeader.mMessageType = MessageType::ChannelListRequest;
    send(message);
}

void Client::askForChannelSubscriptionList() const
{
    Network::Message message;
    message.mHeader.mMessageType = MessageType::ChannelSubscriptionListRequest;
    send(message);
}

void Client::deleteChannel(const std::string_view& channelName) const
{
    Network::Message networkMessage;
    networkMessage.mHeader.mMessageType = MessageType::ChannelDeleteRequest;

    std::string replyInfo;
    replyInfo            = channelName;
    networkMessage.mBody = std::make_any<std::string>(replyInfo);
    send(networkMessage);
}

void Client::askForMessageHistory(const std::uint64_t channelID) const
{
    Network::Message message;
    message.mHeader.mMessageType = MessageType::MessageHistoryRequest;

    message.mBody = std::make_any<std::uint64_t>(channelID);
    send(message);
}

void Client::askForReplyHistory(uint64_t channelID) const
{
    Network::Message message;
    message.mHeader.mMessageType = MessageType::ReplyHistoryRequest;

    message.mBody = std::make_any<std::uint64_t>(channelID);
    send(message);
}

void Client::storeMessage(const std::string& message, const uint64_t channelID) const
{
    Network::Message networkMessage;
    networkMessage.mHeader.mMessageType = MessageType::MessageStoreRequest;

    Models::MessageInfo messageInfo;
    messageInfo._message   = message;
    messageInfo._channelID = channelID;

    networkMessage.mBody = std::make_any<Models::MessageInfo>(messageInfo);
    send(networkMessage);
}

void Client::storeReply(const std::string& message, uint64_t channelID, uint64_t msgID) const
{
    Network::Message networkMessage;
    networkMessage.mHeader.mMessageType = MessageType::ReplyStoreRequest;

    Models::ReplyInfo replyInfo;
    replyInfo._message   = message;
    replyInfo._channelID = channelID;
    replyInfo._msgID     = msgID;

    networkMessage.mBody = std::make_any<Models::ReplyInfo>(replyInfo);
    send(networkMessage);
}

void Client::userRegistration(const std::string& email, const std::string& login, const std::string& password) const
{
    const std::string         pwdHash = Base::Hashing::SHA_256(password, login);
    Models::RegistrationInfo regInfo(email, login, pwdHash);

    Network::Message message;
    message.mHeader.mMessageType = MessageType::RegistrationRequest;
    message.mBody                = std::make_any<Models::RegistrationInfo>(regInfo);

    send(message);
}

void Client::userAuthorization(const std::string& login, const std::string& password) const
{
    const std::string pwdHash = Base::Hashing::SHA_256(password, login);
    Models::LoginInfo loginInfo(login, pwdHash);

    Message message;
    message.mHeader.mMessageType = MessageType::LoginRequest;
    message.mBody                = std::make_any<Models::LoginInfo>(loginInfo);

    send(message);
}

void Client::messageAll() const
{
    Network::Message message;
    message.mHeader.mMessageType = MessageType::MessageAll;
    send(message);
}

void Client::userMessageDelete(const uint64_t messageID) const
{
    Models::MessageInfo messageInfo;
    messageInfo._msgID = messageID;

    Network::Message message;
    message.mHeader.mMessageType = MessageType::MessageDeleteRequest;
    message.mBody                = std::make_any<Models::MessageInfo>(messageInfo);
    send(message);
}

void Client::userMessageDelete(const std::string& messageText) const
{
    Models::MessageInfo messageInfo;
    messageInfo._message = messageText;

    Network::Message message;
    message.mHeader.mMessageType = MessageType::MessageDeleteRequest;
    message.mBody                = std::make_any<Models::MessageInfo>(messageInfo);
    send(message);
}

void Client::subscriptionChannel(const std::uint64_t channelID) const
{
    Network::Message networkMessage;
    networkMessage.mHeader.mMessageType = MessageType::ChannelSubscribeRequest;

    Models::ChannelSubscriptionInfo replyInfo;
    replyInfo._channelID = channelID;
    networkMessage.mBody = std::make_any<Models::ChannelSubscriptionInfo>(replyInfo);
    send(networkMessage);
}

void Client::leaveChannel(const std::string_view& channelName) const
{
    Network::Message networkMessage;
    networkMessage.mHeader.mMessageType = MessageType::ChannelLeaveRequest;

    std::string replyInfo;
    replyInfo            = channelName;
    networkMessage.mBody = std::make_any<std::string>(replyInfo);
    send(networkMessage);
}

void Client::createChannel(const std::string_view& channelName) const
{
    Network::Message networkMessage;
    networkMessage.mHeader.mMessageType = MessageType::ChannelCreateRequest;

    std::string replyInfo;
    replyInfo            = channelName;
    networkMessage.mBody = std::make_any<std::string>(replyInfo);
    send(networkMessage);
}

void Client::createDirectChat(uint64_t receiverId) const
{
    Network::Message networkMessage;
    networkMessage.mHeader.mMessageType = MessageType::DirectMessageCreateRequest;

    networkMessage.mBody = std::make_any<uint64_t>(receiverId);
    send(networkMessage);
}

void Client::userMessageReaction(const std::uint64_t messageID, const std::uint32_t reactionID) const
{
    Models::MessageInfo messageInfo;
    messageInfo._msgID = messageID;
    messageInfo._reactions[reactionID] = std::numeric_limits<std::uint32_t>::max();
    
    Network::Message message;
    message.mHeader.mMessageType = MessageType::MessageReactionRequest;
    message.mBody                = std::make_any<Models::MessageInfo>(messageInfo);

    send(message);
}

void Client::loop()
{
    while (!_incomingMessagesQueue.empty())
    {
        const Message message     = _incomingMessagesQueue.pop_front();
        UtilityTime::consoleLogTimestamp();

        switch (message.mHeader.mMessageType)
        {
            case MessageType::LoginAnswer:
            {
                auto loginSuccessful = std::any_cast<bool>(message.mBody);
                onLoginAnswer(loginSuccessful);
            }
            break;

            case MessageType::ServerAccept:
            {
                _serverAccept = true;
                onServerAccepted();
            }
            break;

            case MessageType::ServerPing:
            {
                timestamp_t timeNow  = RTC::to_time_t(RTC::now());
                timestamp_t timeThen = message.mHeader.mTimestamp;
                onServerPing(std::chrono::duration<double>(timeNow - timeThen).count());
            }
            break;

            case MessageType::ServerMessage:
            {
                /// T\todo add handling 
                uint64_t clientID = 0;
                onServerMessage(clientID);
            }
            break;

            case MessageType::ChannelListRequest:
            {
                auto channelsLists = std::any_cast<std::vector<Models::ChannelInfo>>(message.mBody);
                onChannelListRequest(channelsLists);
            }
            break;

            case MessageType::MessageHistoryAnswer:
            {
                auto historyMessages = std::any_cast<std::vector<Models::MessageInfo>>(message.mBody);
                onMessageHistoryAnswer(historyMessages);
            }
            break;

            case MessageType::MessageStoreAnswer:
            {
                auto storeCode = std::any_cast<Utility::StoringMessageCodes>(message.mBody);
                onMessageStoreAnswer(storeCode);
            }
            break;

            case MessageType::RegistrationAnswer:
            {
                auto registrationCode = std::any_cast<Utility::RegistrationCodes>(message.mBody);
                onRegistrationAnswer(registrationCode);
            }
            break;

            case MessageType::MessageDeleteAnswer:
            {
                auto messageDeleteInfo = std::any_cast<Utility::DeletingMessageCodes>(message.mBody);
                onUserMessageDeleteAnswer(messageDeleteInfo);
            }
            break;

            case MessageType::MessageEditAnswer:
            {
                auto messageEditInfo = std::any_cast<Utility::EditingMessageCodes>(message.mBody);
                onEditMessageAnswer(messageEditInfo);
            }
            break;

            case MessageType::ReplyHistoryAnswer:
            {
                auto repliesHistory = std::any_cast<std::vector<Models::ReplyInfo>>(message.mBody);
                onReplyHistoryAnswer(repliesHistory);
            }
            break;

            case MessageType::ReplyStoreAnswer:
            {
                auto replyStoreCode = std::any_cast<Utility::StoringReplyCodes>(message.mBody);
                onReplyStoreAnswer(replyStoreCode);
            }
            break;

            case MessageType::ChannelSubscribeAnswer:
            {
                auto subscribeCode = std::any_cast<Utility::ChannelSubscribingCodes>(message.mBody);
                onChannelSubscribingAnswer(subscribeCode);
            }
            break;

            case MessageType::ChannelLeaveAnswer:
            {
                auto channelLeaveCode = std::any_cast<Utility::ChannelLeaveCodes>(message.mBody);
                onChannelLeaveAnswer(channelLeaveCode);
            }
            break;

            case MessageType::ChannelSubscriptionListAnswer:
            {
                auto channelsList = std::any_cast<std::vector<uint64_t>>(message.mBody);
                onChannelSubscribingListAnswer(channelsList);
            }
            break;

            case MessageType::ChannelDeleteAnswer:
            {
                auto channelDeleteCode = std::any_cast<Utility::ChannelDeleteCode>(message.mBody);
                onChannelDeleteAnswer(channelDeleteCode);
            }
            break;

            case MessageType::ChannelCreateAnswer:
            {
                auto channelCreateCode = std::any_cast<Utility::ChannelCreateCodes>(message.mBody);
                onChannelCreateAnswer(channelCreateCode);
            }
            break;
            
            case MessageType::MessageReactionAnswer:
            {
                auto messageReactionCode = std::any_cast<Utility::ReactionMessageCodes>(message.mBody);
                onMessageReactionAnswer(messageReactionCode);
            }
            break;

            case MessageType::DirectMessageCreateAnswer:
            {
                auto directMessageCreateAnswer = std::any_cast<Utility::DirectMessageStatus>(message.mBody);
                onDirectMessageCreateAnswer(directMessageCreateAnswer);
            }
            break;

            default:
                Base::Logger::FileLogger::getInstance().log
                (
                    "Unimplemented[" + std::to_string(uint32_t(message.mHeader.mMessageType)) + "]", 
                    Base::Logger::LogLevel::WARNING
                );
        }
    }
}

void Client::onLoginAnswer(bool success)
{
    (void)(success);
    Base::Logger::FileLogger::getInstance().log
    (
        "Login answer is not implemented", 
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onServerAccepted()
{
    Base::Logger::FileLogger::getInstance().log
    (
        "Server accepted is not implemented",
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onServerPing(double timestamp)
{
    (void)(timestamp);
    Base::Logger::FileLogger::getInstance().log
    (
        "Server ping is not implemented",
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onServerMessage(const uint64_t clientId)
{
    (void)(clientId);
    Base::Logger::FileLogger::getInstance().log
    (
        "Server message is not implemented",
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onChannelListRequest(const std::vector<Models::ChannelInfo>& channels)
{
    (void)(channels);
    Base::Logger::FileLogger::getInstance().log
    (
        "Channel list request is not implemented",
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onMessageHistoryAnswer(const std::vector<Models::MessageInfo>& messages)
{
    (void)(messages);
    Base::Logger::FileLogger::getInstance().log
    (
        "Message history answer is not implemented",
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode)
{
    (void)(storingMessageCode);
    Base::Logger::FileLogger::getInstance().log
    (
        "Message store answer is not implemented", 
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onRegistrationAnswer(Utility::RegistrationCodes registrationCode)
{
    (void)(registrationCode);
    Base::Logger::FileLogger::getInstance().log
    (
        "Registration answer is not implemented",
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState)
{
    (void)(deletingState);
    Base::Logger::FileLogger::getInstance().log
    (
        "User message delete answer is not implemented",
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onMessageReactionAnswer(const Utility::ReactionMessageCodes reactionState)
{
    (void)(reactionState);
    Base::Logger::FileLogger::getInstance().log
    (
        "Message reaction answer is not implemented",
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onDisconnect() 
{ 
    Base::Logger::FileLogger::getInstance().log
    (
        "Disconnect is not implemented", 
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onMessageSendFailed(const Message& message) const
{
    (void)(message);
    Base::Logger::FileLogger::getInstance().log
    (
        "Message send failed is not implemented",
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onReplyHistoryAnswer(const std::vector<Models::ReplyInfo>& replies)
{
    (void)(replies);
    Base::Logger::FileLogger::getInstance().log
    (
        "Reply history answer is not implemented", 
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onReplyStoreAnswer(Utility::StoringReplyCodes storingReplyCode)
{
    (void)(storingReplyCode);
    Base::Logger::FileLogger::getInstance().log
    (
        "Reply store answer is not implemented", 
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onChannelLeaveAnswer(Utility::ChannelLeaveCodes ChannelLeaveCode)
{
    (void)(ChannelLeaveCode);
    Base::Logger::FileLogger::getInstance().log
    (
        "Leave channel answer is not implemented",
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onChannelSubscribingAnswer(const Utility::ChannelSubscribingCodes subscribingChannelCode)
{
    (void)(subscribingChannelCode);
    Base::Logger::FileLogger::getInstance().log
    (
        "Subscribing channel answer is not implemented", 
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onChannelSubscribingListAnswer(const std::vector<uint64_t>& subscribingChannelList)
{
    (void)(subscribingChannelList);
    Base::Logger::FileLogger::getInstance().log
    (
        "Subscribing channel list answer is not implemented",
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode)
{
    (void)(channelDeleteCode);
    Base::Logger::FileLogger::getInstance().log
    (
        "Delete channel answer is not implemented",
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onEditMessageAnswer(Utility::EditingMessageCodes reactionState) const
{
    (void)(reactionState);
    Base::Logger::FileLogger::getInstance().log
    (
        "Editing message answer is not implemented",
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode)
{
    (void)(channelCreateCode);
    Base::Logger::FileLogger::getInstance().log
    (
        "Create channel answer is not implemented",
        Base::Logger::LogLevel::WARNING
    );
}

void Client::onDirectMessageCreateAnswer(Utility::DirectMessageStatus directMessageCreateAnswer)
{
    (void)(directMessageCreateAnswer);
    Base::Logger::FileLogger::getInstance().log
    (
        "Direct message create answer is not implemented",
        Base::Logger::LogLevel::WARNING
    );
}
}  // namespace Network
