#include "MockClient.hpp"

namespace MockClient
{
bool MockClient::connectToServer(const std::string_view& host, const uint16_t port)
{
    if (host != ServerInfo::Address::remote && host != ServerInfo::Address::local)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Bad server address",
            Base::Logger::LogLevel::ERR
        );

        return false;
    }

    if (port == ServerInfo::Port::test && port != ServerInfo::Port::production)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Bad port value",
            Base::Logger::LogLevel::ERR
        );

        return false;
    }

    asio::ip::tcp::resolver resolver(testContext);

    using OwnerType = Connection::OwnerType;
    testConnection =
        std::make_unique<Connection>(OwnerType::CLIENT, testContext, 
            asio::ip::tcp::socket(testContext), testIncomingMessagesQueue);

    try
    {
        auto endpoints = resolver.resolve(host, std::to_string(port));
        testConnection->connectToServer(endpoints);
        testContextThread = std::thread([=]() {
            while (testContext.run_one())
            {
                loop();
            }
            serverAccept = false;
            onDisconnect(); 
        });
    }
    catch (const std::exception& exception)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Client Exception: " + std::string(exception.what()),
            Base::Logger::LogLevel::ERR
        );

        return false;
    }

    return true;
}

void MockClient::disconnectFromServer() 
{
    if (isConnected())
    {
        testConnection->disconnect();
    }

    testContext.stop();

    if (testContextThread.get_id() != std::this_thread::get_id() && testContextThread.joinable())
    {
        testContextThread.join();
    }

    testConnection.reset();
}

bool MockClient::isConnected() const 
{
    if (testConnection != nullptr)
    {
        return testConnection->isConnected() && serverAccept;
    }

    return false;
}

void MockClient::send(const Message& message) const 
{
    if (isConnected())
    {
        testConnection->send(message);
    }
    else
    {
        onMessageSendFailed();
    }
}

SafeQueue<Message>& MockClient::getMessage()
{
    return testIncomingMessagesQueue;
}

void MockClient::onLoginAnswer(bool success) const
{
    enum class LoginState
    {
        SUCCESS,
        IN_PROGRESS,
        FAILED
    };

    static LoginState loginState;

    if (success)
    {
        loginState = LoginState::SUCCESS;

        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] The user is logged in!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        loginState = LoginState::FAILED;

        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] User is not logged in!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onServerAccepted() const
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[CLIENT] Server accepted the connection!", 
        Base::Logger::LogLevel::INFO
    );
}

void MockClient::onServerPing(double timestamp) const
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[CLIENT] Ping: " + std::to_string(timestamp), 
        Base::Logger::LogLevel::INFO
    );
}

void MockClient::onServerMessage(const uint64_t clientID) const
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[CLIENT] Hello from [" + std::to_string(clientID) + "]", 
        Base::Logger::LogLevel::INFO
    );
}

void MockClient::onChannelListRequest(const std::vector<Models::ChannelInfo>& channels) const
{
    auto result = Utility::StoringReplyCodes::SUCCESS;
    if (!channels.empty())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Channel list sent successfully!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        result = Utility::StoringReplyCodes::FAILED;        
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Channel list isn't implemented!",
            Base::Logger::LogLevel::INFO
        );
     }
}

void MockClient::onMessageHistoryAnswer(const std::vector<Models::MessageInfo>& messages) const
{
    auto result = Utility::StoringMessageCodes::SUCCESS;
    if (!messages.empty())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Message history is successfully!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        result = Utility::StoringMessageCodes::FAILED;
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Message history isn't implemented!",
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode) const
{
    if (storingMessageCode == Utility::StoringMessageCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Message sent successfully!",
            Base::Logger::LogLevel::INFO
        );
    }
    else if (storingMessageCode == Utility::StoringMessageCodes::FAILED)
    {
         Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Message not sent!:(!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onRegistrationAnswer(Utility::RegistrationCodes registrationCode) const
{
   
    if (registrationCode == Utility::RegistrationCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] A user has registered!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] The user already exists", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState) const
{
    if (deletingState == Utility::DeletingMessageCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] SUCCESS DELETING", 
            Base::Logger::LogLevel::INFO
        );
    }
    else if (deletingState == Utility::DeletingMessageCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] FAILED DELETING", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] UNKNOWN deleting message code", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onMessageReactionAnswer(const Utility::ReactionMessageCodes reactionState) const
{
    if (reactionState == Utility::ReactionMessageCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Reaction update SUCCESS", 
            Base::Logger::LogLevel::INFO
        );
    }
    else if (reactionState == Utility::ReactionMessageCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Reaction update FAILED", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onEditMessageAnswer(Utility::EditingMessageCodes reactionState) const
{
    if (reactionState == Utility::EditingMessageCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] SUCCESS editing message",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Bad editing message",
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onDisconnect() const
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[CLIENT] You've been disconnected from server!", 
        Base::Logger::LogLevel::INFO
    );
}

void MockClient::onMessageSendFailed() const
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[CLIENT] Message send failed is not implemented", 
        Base::Logger::LogLevel::INFO
    );
}

void MockClient::onReplyHistoryAnswer(Utility::StoringReplyCodes storingReplyCode) const
{
    if (storingReplyCode == Utility::StoringReplyCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] SUCCESS sending",
            Base::Logger::LogLevel::INFO
        );
    }
    else if (storingReplyCode == Utility::StoringReplyCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] FAILED sending",
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onReplyStoreAnswer(Utility::StoringReplyCodes storingReplyCode) const
{
    if (storingReplyCode == Utility::StoringReplyCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] SUCCESS sending", 
            Base::Logger::LogLevel::INFO
        );
    }
    else if (storingReplyCode == Utility::StoringReplyCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] FAILED sending", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onChannelLeaveAnswer(Utility::ChannelLeaveCodes ChannelLeaveCode) const
{
    if (ChannelLeaveCode == Utility::ChannelLeaveCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] SUCCESS LEAVING",
            Base::Logger::LogLevel::INFO
        );
    }
    else if (ChannelLeaveCode == Utility::ChannelLeaveCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] FAILED LEAVING", 
            Base::Logger::LogLevel::INFO
        );
    }
    else if (ChannelLeaveCode == Utility::ChannelLeaveCodes::CHANNEL_NOT_FOUND)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] CHANNEL NOT FOUND", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onChannelSubscribingAnswer(Utility::ChannelSubscribingCodes subscribingChannelCode) const
{
    if (subscribingChannelCode == Utility::ChannelSubscribingCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] SUCCESS SUBSCRIBING",
            Base::Logger::LogLevel::INFO
        );
    }
    else if (subscribingChannelCode == Utility::ChannelSubscribingCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] FAILED SUBSCRIBING", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onChannelSubscribingListAnswer(const std::vector<uint64_t> subscribingChannelList) const
{
    if (!subscribingChannelList.empty())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Channel subscribing list is successfully!!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Channel subscribing list isn't implementet!!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode) const
{
    if (channelDeleteCode == Utility::ChannelDeleteCode::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] SUCCESS DELETING",
            Base::Logger::LogLevel::INFO
        );
    }
    else if (channelDeleteCode == Utility::ChannelDeleteCode::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] FAILED DELETING", 
            Base::Logger::LogLevel::INFO
        );
    }
    else if (channelDeleteCode == Utility::ChannelDeleteCode::CHANNEL_NOT_FOUND)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] CHANNEL NOT FOUND", 
            Base::Logger::LogLevel::INFO
        );
    }
    else if (channelDeleteCode == Utility::ChannelDeleteCode::CHANNEL_IS_NOT_USER)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] CHANNEL DOES NOT BELONG TO THE USER", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode) const
{
    if (channelCreateCode == Utility::ChannelCreateCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] SUCCESS CREATING", 
            Base::Logger::LogLevel::INFO
        );
    }
    else if (channelCreateCode == Utility::ChannelCreateCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] FAILD CREATING", 
            Base::Logger::LogLevel::INFO
        );
    }
    else if (channelCreateCode == Utility::ChannelCreateCodes::CHANNEL_ALREADY_CREATED)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] CHANNEL ALREADY CREATE",
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onDirectMessageCreateAnswer(Utility::DirectMessageStatus directMessageCreateAnswer) const
{
    if (directMessageCreateAnswer == Utility::DirectMessageStatus::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] SUCCESS CREATING", 
            Base::Logger::LogLevel::INFO
        );
    }
    else if (directMessageCreateAnswer == Utility::DirectMessageStatus::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] FAILED CREATING", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::loop()
{
    while (!testIncomingMessagesQueue.empty())
    {
        const Message message = testIncomingMessagesQueue.pop_front();

        switch (message.mHeader.mMessageType)
        {
            case Message::MessageType::LoginAnswer:
            {
                auto loginSuccessful = std::any_cast<bool>(message.mBody);
                onLoginAnswer(loginSuccessful);
            }
            break;

            case Message::MessageType::ServerAccept:
            {
                serverAccept = true;
                onServerAccepted();
            }
            break;

            case Message::MessageType::ServerPing:
            {
                timestamp_t timeNow  = RTC::to_time_t(RTC::now());
                timestamp_t timeThen = message.mHeader.mTimestamp;
                onServerPing(std::chrono::duration<double>(timeNow - timeThen).count());
            }
            break;

            case Message::MessageType::ServerMessage:
            {
                uint64_t clientID = 0;
                onServerMessage(clientID);
            }
            break;

            case Message::MessageType::ChannelListRequest:
            {
                auto channels = std::any_cast<std::vector<Models::ChannelInfo>>(message.mBody);
                onChannelListRequest(channels);
            }
            break;

            case Message::MessageType::MessageHistoryAnswer:
            {
                auto messages = std::any_cast<std::vector<Models::MessageInfo>>(message.mBody);
                onMessageHistoryAnswer(messages);
            }
            break;

            case Message::MessageType::MessageStoreAnswer:
            {
                auto code = std::any_cast<Utility::StoringMessageCodes>(message.mBody);
                onMessageStoreAnswer(code);
            }
            break;

            case Message::MessageType::RegistrationAnswer:
            {
                auto code = std::any_cast<Utility::RegistrationCodes>(message.mBody);
                onRegistrationAnswer(code);
            }
            break;

            case Message::MessageType::MessageDeleteAnswer:
            {
                auto messageInfo = std::any_cast<Utility::DeletingMessageCodes>(message.mBody);
                onUserMessageDeleteAnswer(messageInfo);
            }
            break;

            case Message::MessageType::MessageEditAnswer:
            {
                auto messageEdit = std::any_cast<Utility::EditingMessageCodes>(message.mBody);
                onEditMessageAnswer(messageEdit);
            }
            break;

            case Message::MessageType::ReplyHistoryAnswer:
            {
                auto replies = std::any_cast<Utility::StoringReplyCodes>(message.mBody);
                onReplyHistoryAnswer(replies);
            }
            break;

            case Message::MessageType::ReplyStoreAnswer:
            {
                auto code = std::any_cast<Utility::StoringReplyCodes>(message.mBody);
                onReplyStoreAnswer(code);
            }
            break;

            case Message::MessageType::ChannelSubscribeAnswer:
            {
                auto code = std::any_cast<Utility::ChannelSubscribingCodes>(message.mBody);
                onChannelSubscribingAnswer(code);
            }
            break;

            case Message::MessageType::ChannelLeaveAnswer:
            {
                auto ChannelLeaveCode = std::any_cast<Utility::ChannelLeaveCodes>(message.mBody);
                onChannelLeaveAnswer(ChannelLeaveCode);
            }
            break;

            case Message::MessageType::ChannelSubscriptionListAnswer:
            {
                auto channelsList = std::any_cast<std::vector<uint64_t>>(message.mBody);
                onChannelSubscribingListAnswer(channelsList);
            }
            break;

            case Message::MessageType::ChannelDeleteAnswer:
            {
                auto channelDeleteCode = std::any_cast<Utility::ChannelDeleteCode>(message.mBody);
                onChannelDeleteAnswer(channelDeleteCode);
            }
            break;

            case Message::MessageType::ChannelCreateAnswer:
            {
                auto channelCreateCode = std::any_cast<Utility::ChannelCreateCodes>(message.mBody);
                onChannelCreateAnswer(channelCreateCode);
            }
            break;

            case Message::MessageType::MessageReactionAnswer:
            {
                auto messageInfo = std::any_cast<Utility::ReactionMessageCodes>(message.mBody);
                onMessageReactionAnswer(messageInfo);
            }
            break;

            case Message::MessageType::DirectMessageCreateAnswer:
            {
                auto directMessageCreateAnswer = std::any_cast<Utility::DirectMessageStatus>(message.mBody);
                onDirectMessageCreateAnswer(directMessageCreateAnswer);
            }
            break;

            default:
                Base::Logger::FileLogger::getInstance().log
                (
                    "Unimplemented[" +
                    std::to_string(uint32_t(message.mHeader.mMessageType)) + "]",
                    Base::Logger::LogLevel::WARNING
                );
        }
    }
} 
}  // namespace MockClient
