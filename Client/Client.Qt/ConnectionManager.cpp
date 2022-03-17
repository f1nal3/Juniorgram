#include "ConnectionManager.hpp"

#include "Application.hpp"
#include "Settings.hpp"

ReceiverManager* ReceiverManager::self;

ReceiverManager::ReceiverManager() { self = this; }

ConnectionManager::ConnectionManager() { configureConnectionProperties(); }

void ConnectionManager::init()
{
    Settings::getInstance().beginGroup("ConnectionSettings");
    std::string host = Settings::getInstance().value("address").toString().toStdString();
    auto port = Settings::getInstance().value("port").toInt();
    Settings::getInstance().endGroup();

    connectToServer
    (
        host.data(),
        port
    );
}

void ConnectionManager::onServerAccepted()
{
    oApp->showMessage("Connection Status", "Server accepted the connection!");
    emit ReceiverManager::instance()->onServerAccepted();
}

void ConnectionManager::onServerPing(double timestamp)
{
    Base::Logger::FileLogger::getInstance().log
    (
        "Ping: " + std::to_string(timestamp), 
        Base::Logger::LogLevel::INFO
    );
    emit ReceiverManager::instance()->onServerPing(timestamp);
}

void ConnectionManager::onServerMessage(const uint64_t clientId)
{
    Base::Logger::FileLogger::getInstance().log
    (
        "Hello from [" + std::to_string(clientId) + "]", 
        Base::Logger::LogLevel::INFO
    );
    emit ReceiverManager::instance()->onServerMessage(clientId);
}

void ConnectionManager::onChannelListRequest(const std::vector<Network::ChannelInfo>& channels)
{
    qRegisterMetaType<std::vector<Network::ChannelInfo>>("std::vector<Network::ChannelInfo>");
    emit ReceiverManager::instance()->onChannelListRequest(channels);
}

void ConnectionManager::onMessageHistoryAnswer(const std::vector<Network::MessageInfo>& messages)
{
    qRegisterMetaType<std::vector<Network::MessageInfo>>("std::vector<Network::MessageInfo>");
    emit ReceiverManager::instance()->onMessageHistoryAnswer(messages);
}

void ConnectionManager::onReplyHistoryAnswer(const std::vector<Network::ReplyInfo>& replies)
{
    qRegisterMetaType<std::vector<Network::ReplyInfo>>("std::vector<Network::ReplyInfo>");
    emit ReceiverManager::instance()->onReplyHistoryAnswer(replies);
}

void ConnectionManager::onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode)
{
    if (storingMessageCode == Utility::StoringMessageCodes::SUCCESS)
        oApp->showMessage("Message", "Message sent successfully!");
    else if (storingMessageCode == Utility::StoringMessageCodes::FAILED)
        oApp->showMessage("Message", "Message not sent!:(", MessageType::Error);
    qRegisterMetaType<Utility::StoringMessageCodes>("Utility::StoringMessageCodes");
    emit ReceiverManager::instance()->onMessageStoreAnswer(storingMessageCode);
}

void ConnectionManager::onReplyStoreAnswer(Utility::StoringReplyCodes storingReplyCode)
{
    if (storingReplyCode == Utility::StoringReplyCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log("SUCCESS sending", Base::Logger::LogLevel::INFO);
    }
    else if (storingReplyCode == Utility::StoringReplyCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log("FAILED sending", Base::Logger::LogLevel::INFO);
    }
    qRegisterMetaType<Utility::StoringReplyCodes>("Utility::StoringReplyCodes");
    emit ReceiverManager::instance()->onReplyStoreAnswer(storingReplyCode);
}

void ConnectionManager::onMessageReactionAnswer(const Utility::ReactionMessageCodes reactionCode)
{
    if (reactionCode == Utility::ReactionMessageCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log("Reaction update SUCCESS", Base::Logger::LogLevel::INFO);
    }
    else if (reactionCode == Utility::ReactionMessageCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log("Reaction update FAILED", Base::Logger::LogLevel::INFO);
    }
    qRegisterMetaType<Utility::ReactionMessageCodes>("Utility::ReactionMessageCodes");
    emit ReceiverManager::instance()->onMessageReactionAnswer(reactionCode);
}

void ConnectionManager::onRegistrationAnswer(Utility::RegistrationCodes registrationCode)
{
    qRegisterMetaType<Utility::RegistrationCodes>("Utility::RegistrationCodes");
    emit ReceiverManager::instance()->onRegistrationAnswer(registrationCode);
}

void ConnectionManager::onLoginAnswer(bool success)
{
    if (success)
    {
        loginState = LoginState::SUCCESS;
    }
    else
    {
        loginState = LoginState::FAILED;
    }
    emit ReceiverManager::instance()->onLoginAnswer(success);
}

void ConnectionManager::onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingCode)
{
    if (deletingCode == Utility::DeletingMessageCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log("SUCCESS DELETING", Base::Logger::LogLevel::INFO);
    }
    else if (deletingCode == Utility::DeletingMessageCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log("FAILED DELETING", Base::Logger::LogLevel::INFO);
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log("UNKNOWN deleting message code", Base::Logger::LogLevel::INFO);
    }
    qRegisterMetaType<Utility::DeletingMessageCodes>("Utility::DeletingMessageCodes");
    emit ReceiverManager::instance()->onUserMessageDeleteAnswer(deletingCode);
}

void ConnectionManager::onChannelLeaveAnswer(Utility::ChannelLeaveCodes ChannelLeaveCode)
{
    if (ChannelLeaveCode == Utility::ChannelLeaveCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log("SUCCESS LEAVING", Base::Logger::LogLevel::INFO);
    }
    else if (ChannelLeaveCode == Utility::ChannelLeaveCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log("FAILED LEAVING", Base::Logger::LogLevel::INFO);
    }
    else if (ChannelLeaveCode == Utility::ChannelLeaveCodes::CHANNEL_NOT_FOUND)
    {
        Base::Logger::FileLogger::getInstance().log("CHANNEL NOT FOUND", Base::Logger::LogLevel::INFO);
    }
    qRegisterMetaType<Utility::ChannelLeaveCodes>("Utility::ChannelLeaveCodes");
    emit ReceiverManager::instance()->onChannelLeaveAnswer(ChannelLeaveCode);
}

void ConnectionManager::onChannelSubscribingAnswer(const Utility::ChannelSubscribingCodes subscribingChannelCode)
{
    if (subscribingChannelCode == Utility::ChannelSubscribingCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log("SUCCESS SUBSCRIBING", Base::Logger::LogLevel::INFO);
    }
    else if (subscribingChannelCode == Utility::ChannelSubscribingCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log("FAILED SUBSCRIBING", Base::Logger::LogLevel::INFO);
    }
    qRegisterMetaType<Utility::ChannelSubscribingCodes>("Utility::ChannelSubscribingCodes");
    emit ReceiverManager::instance()->onChannelSubscriptionAnswer(subscribingChannelCode);
}

void ConnectionManager::onChannelSubscribingListAnswer(const std::vector<uint64_t> subscribingChannelList)
{
    qRegisterMetaType<std::vector<uint64_t>>("std::vector<uint64_t>");
    emit ReceiverManager::instance()->onChannelSubscriptionListAnswer(subscribingChannelList);
}

void ConnectionManager::onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode)
{
    if (channelDeleteCode == Utility::ChannelDeleteCode::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log("SUCCESS DELETING", Base::Logger::LogLevel::INFO);
    }
    else if (channelDeleteCode == Utility::ChannelDeleteCode::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log("FAILED DELETING", Base::Logger::LogLevel::INFO);
    }
    else if (channelDeleteCode == Utility::ChannelDeleteCode::CHANNEL_NOT_FOUND)
    {
        Base::Logger::FileLogger::getInstance().log("CHANNEL NOT FOUND", Base::Logger::LogLevel::INFO);
    }
    else if (channelDeleteCode == Utility::ChannelDeleteCode::CHANNEL_IS_NOT_USER)
    {
        Base::Logger::FileLogger::getInstance().log("CHANNEL DOES NOT BELONG TO THE USER", Base::Logger::LogLevel::INFO);
    }
    qRegisterMetaType<Utility::ChannelDeleteCode>("Utility::ChannelDeleteCode");
    emit ReceiverManager::instance()->onChannelDeleteAnswer(channelDeleteCode);
}

void ConnectionManager::onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode)
{
    if (channelCreateCode == Utility::ChannelCreateCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log("SUCCESS CREATING", Base::Logger::LogLevel::INFO);
    }
    else if (channelCreateCode == Utility::ChannelCreateCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log("FAILD CREATING", Base::Logger::LogLevel::INFO);
    }
    else if (channelCreateCode == Utility::ChannelCreateCodes::CHANNEL_ALREADY_CREATED)
    {
        Base::Logger::FileLogger::getInstance().log("CHANNEL ALREADY CREATE", Base::Logger::LogLevel::INFO);
    }
    qRegisterMetaType<Utility::ChannelCreateCodes>("Utility::ChannelCreateCodes");
    emit ReceiverManager::instance()->onChannelCreateAnswer(channelCreateCode);
}

void ConnectionManager::onDirectMessageCreateAnswer(Utility::DirectMessageStatus directMessageCreateAnswer)
{
    if (directMessageCreateAnswer == Utility::DirectMessageStatus::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log("SUCCESS CREATING", Base::Logger::LogLevel::INFO);
    }
    else if (directMessageCreateAnswer == Utility::DirectMessageStatus::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log("FAILED CREATING", Base::Logger::LogLevel::INFO);
    }
    qRegisterMetaType<Utility::DirectMessageStatus>("Utility::DirectMessageStatus");
    emit ReceiverManager::instance()->onDirectMessageCreateAnswer(directMessageCreateAnswer);
}

void ConnectionManager::onDisconnect()
{
    disconnectFromServer();
    loginState = LoginState::FAILED;
    oApp->showMessage("Connection Status", "You've been disconnected from server!", MessageType::Error);
    emit ReceiverManager::instance()->onDisconnect();
}

void ConnectionManager::configureConnectionProperties()
{
    using Values = std::map<QString, QVariant>;

    Settings::getInstance().configureSettings
    (
        "ConnectionSettings",
        Values
        {
            { "address", ServerInfo::Address::remote.data() },
            { "port"   , ServerInfo::Port::production }
        }
    );
}
