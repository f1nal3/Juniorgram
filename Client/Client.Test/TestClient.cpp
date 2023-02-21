#include "TestClient.hpp"

namespace TestObject
{
using MessageType = Network::Message::MessageType;
using UtilityTime::timestamp_t;
using UtilityTime::RTC;

TestClient::~TestClient() noexcept { disconnectFromServer(); }

bool TestClient::connectToServer(const std::string_view& host, const uint16_t port)
{ 
    checkConnectionArguments(host, port);

    using OwnerType = Connection::OwnerType;
    _connection     = std::make_unique<Connection>(OwnerType::CLIENT, _context, 
        asio::ip::tcp::socket(_context), _incomingMessagesQueue);
    asio::ip::tcp::resolver resolver(_context);

    try
    {
        auto endpoints = resolver.resolve(host, std::to_string(port));
        _connection->connectToServer(endpoints);

        checkServerAcception();

        _contextThread = std::thread([this]() {
            while (_context.run_one())
            {
                noose();
            }
            _serverAccept = false;
            onDisconnect();
        });
    }
    catch (const std::exception& exception)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "Client Exception: " 
            + std::string(exception.what()), 
            Base::Logger::LogLevel::ERR
        );
        return false;
    }
    return true; 
}

void TestClient::disconnectFromServer()
{
    if (isConnected())
    {
        _connection->disconnect();
    }

    _context.stop();

    if (_contextThread.get_id() != std::this_thread::get_id()
        && (_contextThread.joinable()))
    {
        _contextThread.join();
    }

    _connection.reset();
}

bool TestClient::isConnected() const
{
    if (_connection != nullptr)
    {
        return _connection->isConnected() && _serverAccept;
    }
    return false;
}

void TestClient::send(const Message& message) const
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

bool TestClient::checkConnectionArguments(const std::string_view& host, const uint16_t port) const
{
    if (host != TestServerInfo::Address::local || port != TestServerInfo::Port::test)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "Bad arguments", 
            Base::Logger::LogLevel::ERR
        );

        return false;
    }
    return true;
}

bool TestClient::checkServerAcception()
{
    while (_serverAccept == false)
    {
        _context.run_one();
        noose();
    }
    return _serverAccept;
}

void TestClient::noose()
{
     while (!_incomingMessagesQueue.empty())
     {
        UtilityTime::consoleLogTimestamp();
        const Message message     = _incomingMessagesQueue.pop_front();

        switch (message.mHeader.mMessageType)
        {
            case MessageType::ServerAccept:
            {
                _serverAccept = true;
                onServerAccepted();
            }
            break;

            case MessageType::RegistrationAnswer:
            {
                auto registrationCode = std::any_cast<Utility::RegistrationCodes>(message.mBody);
                onRegistrationAnswer(registrationCode);
            }
            break;

            case MessageType::LoginAnswer:
            {
                auto loginSuccessful = std::any_cast<bool>(message.mBody);
                onLoginAnswer(loginSuccessful);
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
                uint64_t clientID = 0;
                onServerMessage(clientID);
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

            case MessageType::ChannelListRequest:
            {
                auto channelsLists = std::any_cast<std::vector<Models::ChannelInfo>>(message.mBody);
                onChannelListRequest(channelsLists);
            }
            break;

            case MessageType::ChannelSubscriptionListAnswer:
            {
                auto channelsList = std::any_cast<std::vector<uint64_t>>(message.mBody);
                onChannelSubscribingListAnswer(channelsList);
            }
            break;

            case MessageType::ChannelSubscribeAnswer:
            {
                auto subscribeCode = std::any_cast<Utility::ChannelSubscribingCodes>(message.mBody);
                onChannelSubscribingAnswer(subscribeCode);
            }
            break;

            case MessageType::ChannelCreateAnswer:
            {
                auto channelCreateCode = std::any_cast<Utility::ChannelCreateCodes>(message.mBody);
                onChannelCreateAnswer(channelCreateCode);
            }
            break;
            
            case MessageType::ChannelDeleteAnswer:
            {
                auto channelDeleteCode = std::any_cast<Utility::ChannelDeleteCode>(message.mBody);
                onChannelDeleteAnswer(channelDeleteCode);
            }
            break;

            case MessageType::ChannelLeaveAnswer:
            {
                auto channelLeaveCode = std::any_cast<Utility::ChannelLeaveCodes>(message.mBody);
                onChannelLeaveAnswer(channelLeaveCode);
            }
            break;

            default:
            {
                Base::Logger::FileLogger::getInstance().log
                (
                    "Unimplemented Message[" 
                    + std::to_string(uint32_t(message.mHeader.mMessageType)) + "]",
                    Base::Logger::LogLevel::WARNING
                );
            }
            break;
        }
     }
}

void TestClient::onLoginAnswer(bool success) const
{
    if (success)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] The user is logged in!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] User is not logged in!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onServerAccepted() const
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[TestClient] Server accepted the connection!", 
        Base::Logger::LogLevel::INFO
    );
}

void TestClient::onServerPing(double timestamp) const
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[TestClient] Ping: " + std::to_string(timestamp), 
        Base::Logger::LogLevel::INFO
    );
}

void TestClient::onServerMessage(const uint64_t clientID) const
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[TestClient] Hello from [" + std::to_string(clientID) + "]", 
        Base::Logger::LogLevel::INFO
    );
}

void TestClient::onChannelListRequest(const std::vector<Models::ChannelInfo>& channels) const
{
    if (!channels.empty())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Channel list sent successfully!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Channel list isn't implemented!",
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onMessageHistoryAnswer(const std::vector<Models::MessageInfo>& messages) const
{
    if (messages.empty())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Message history isn't implemented!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Message history is taken successfully!",
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode) const
{
    if (storingMessageCode == Utility::StoringMessageCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Message sent successfully!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
         Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Message didn't sent!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onRegistrationAnswer(Utility::RegistrationCodes registrationCode) const
{
    if (registrationCode == Utility::RegistrationCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] A user has registered!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] The user already exists!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState) const
{
    if (deletingState == Utility::DeletingMessageCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Success deleting!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Failed deleting!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onMessageReactionAnswer(const Utility::ReactionMessageCodes reactionState) const
{
    if (reactionState == Utility::ReactionMessageCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Reaction updating success!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Reaction updating failed", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onEditMessageAnswer(Utility::EditingMessageCodes reactionState) const
{
    if (reactionState == Utility::EditingMessageCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Success editing message!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Bad editing message!",
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onDisconnect() const
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[TestClient] You've been disconnected from server!", 
        Base::Logger::LogLevel::INFO
    );
}

void TestClient::onMessageSendFailed(const Message& message) const
{
    (void)(message);
    Base::Logger::FileLogger::getInstance().log
    (   
        "[TestClient] Message send failed is not implemented!", 
        Base::Logger::LogLevel::INFO
    );
}

void TestClient::onReplyHistoryAnswer(const std::vector<Models::ReplyInfo>& replies) const
{
    if (!replies.empty())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Reply history answer is implemented!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Reply history answer isn't implemented!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onReplyStoreAnswer(Utility::StoringReplyCodes storingReplyCode) const
{
    if (storingReplyCode == Utility::StoringReplyCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Success sending!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Failed sending!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onChannelLeaveAnswer(Utility::ChannelLeaveCodes ChannelLeaveCode) const
{
    if (ChannelLeaveCode == Utility::ChannelLeaveCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Success leaving!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Failed leaving!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onChannelSubscribingAnswer(const Utility::ChannelSubscribingCodes subscribingChannelCode) const
{
    if (subscribingChannelCode == Utility::ChannelSubscribingCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Success subscribing",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Failed subscribing", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onChannelSubscribingListAnswer(const std::vector<uint64_t>& subscribingChannelList) const
{
    if (!subscribingChannelList.empty())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Channel subscribing list is successfully!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Channel subscribing list isn't implementet!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode) const
{
    if (channelDeleteCode == Utility::ChannelDeleteCode::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Success deleting!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Failed deleting!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode) const
{
    if (channelCreateCode == Utility::ChannelCreateCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Success creating!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Faild creating!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onDirectMessageCreateAnswer(Utility::DirectMessageStatus directMessageCreateAnswer) const
{
    if (directMessageCreateAnswer == Utility::DirectMessageStatus::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Success creating!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[TestClient] Failed creating!", 
            Base::Logger::LogLevel::INFO
        );
    }
}
}  /// namespace TestObject
