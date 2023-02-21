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

void TestClient::disconnectFromServer()
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

bool TestClient::checkConnectionArguments(const std::string_view& host, const uint16_t port)
{
    if (host != TestServerInfo::Address::remote && host != TestServerInfo::Address::local ||
        (port != TestServerInfo::Port::test && port != TestServerInfo::Port::production))
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
        loop();
    }
    return _serverAccept;
}

void TestClient::loop()
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

void TestClient::onLoginAnswer(bool success)
{
    if (success)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] The user is logged in!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] User is not logged in!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onServerAccepted()
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[MockClient] Server accepted the connection!", 
        Base::Logger::LogLevel::INFO
    );
}

void TestClient::onServerPing(double timestamp)
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[MockClient] Ping: " + std::to_string(timestamp), 
        Base::Logger::LogLevel::INFO
    );
}

void TestClient::onServerMessage(const uint64_t clientID)
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[MockClient] Hello from [" + std::to_string(clientID) + "]", 
        Base::Logger::LogLevel::INFO
    );
}

void TestClient::onChannelListRequest(const std::vector<Models::ChannelInfo>& channels)
{
    if (!channels.empty())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Channel list sent successfully!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Channel list isn't implemented!",
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onMessageHistoryAnswer(const std::vector<Models::MessageInfo>& messages)
{
    if (messages.empty())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Message history isn't implemented!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Message history is taken successfully!",
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode)
{
    if (storingMessageCode == Utility::StoringMessageCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Message sent successfully!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
         Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Message didn't sent!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onRegistrationAnswer(Utility::RegistrationCodes registrationCode)
{
    if (registrationCode == Utility::RegistrationCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] A user has registered!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] The user already exists!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState)
{
    if (deletingState == Utility::DeletingMessageCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Success deleting!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Failed deleting!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onMessageReactionAnswer(const Utility::ReactionMessageCodes reactionState)
{
    if (reactionState == Utility::ReactionMessageCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Reaction updating success!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Reaction updating failed", 
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
            "[MockClient] Success editing message!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Bad editing message!",
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onDisconnect()
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[MockClient] You've been disconnected from server!", 
        Base::Logger::LogLevel::INFO
    );
}

void TestClient::onMessageSendFailed(const Message& message) const
{
    (void)(message);
    Base::Logger::FileLogger::getInstance().log
    (   
        "[MockClient] Message send failed is not implemented!", 
        Base::Logger::LogLevel::INFO
    );
}

void TestClient::onReplyHistoryAnswer(const std::vector<Models::ReplyInfo>& replies)
{
    if (!replies.empty())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Reply history answer is implemented!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Reply history answer isn't implemented!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onReplyStoreAnswer(Utility::StoringReplyCodes storingReplyCode)
{
    if (storingReplyCode == Utility::StoringReplyCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Success sending!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Failed sending!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onChannelLeaveAnswer(Utility::ChannelLeaveCodes ChannelLeaveCode)
{
    if (ChannelLeaveCode == Utility::ChannelLeaveCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Success leaving!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Failed leaving!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onChannelSubscribingAnswer(const Utility::ChannelSubscribingCodes subscribingChannelCode)
{
    if (subscribingChannelCode == Utility::ChannelSubscribingCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Success subscribing",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Failed subscribing", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onChannelSubscribingListAnswer(const std::vector<uint64_t>& subscribingChannelList)
{
    if (!subscribingChannelList.empty())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Channel subscribing list is successfully!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Channel subscribing list isn't implementet!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode)
{
    if (channelDeleteCode == Utility::ChannelDeleteCode::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Success deleting!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Failed deleting!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode)
{
    if (channelCreateCode == Utility::ChannelCreateCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Success creating!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Faild creating!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void TestClient::onDirectMessageCreateAnswer(Utility::DirectMessageStatus directMessageCreateAnswer)
{
    if (directMessageCreateAnswer == Utility::DirectMessageStatus::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Success creating!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Failed creating!", 
            Base::Logger::LogLevel::INFO
        );
    }
}
}  /// namespace TestObject
