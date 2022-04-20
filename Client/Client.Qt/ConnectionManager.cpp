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

void ConnectionManager::onReconnect()
{ 
    //ToDo: Fooes onResume && onPause from Reconnect Part
    oApp->showMessage("Connection Status", "Server trying to connection!", MessageType::Warning);
    
    reconnectToServer();
    emit ReceiverManager::instance()->onServerAccepted();
}

void ConnectionManager::onServerAccepted()
{
    oApp->showMessage("Connection Status", "Server accepted the connection!");
    emit ReceiverManager::instance()->onServerAccepted();
}

void ConnectionManager::onServerPing(double timestamp)
{
    std::cout << "Ping: " << timestamp << "\n";
    emit ReceiverManager::instance()->onServerPing(timestamp);
}

void ConnectionManager::onServerMessage(const uint64_t clientId)
{
    std::cout << "Hello from [" << clientId << "]\n";
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
        std::cout << "SUCCESS sending" << std::endl;
    }
    else if (storingReplyCode == Utility::StoringReplyCodes::FAILED)
    {
        std::cout << "FAILED sending" << std::endl;
    }
    qRegisterMetaType<Utility::StoringReplyCodes>("Utility::StoringReplyCodes");
    emit ReceiverManager::instance()->onReplyStoreAnswer(storingReplyCode);
}

void ConnectionManager::onMessageReactionAnswer(const Utility::ReactionMessageCodes reactionCode)
{
    if (reactionCode == Utility::ReactionMessageCodes::SUCCESS)
    {
        std::cout << "Reaction update SUCCESS\n";
    }
    else if (reactionCode == Utility::ReactionMessageCodes::FAILED)
    {
        std::cout << "Reaction update FAILED\n";
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
        std::cout << "SUCCESS DELETING" << std::endl;
    }
    else if (deletingCode == Utility::DeletingMessageCodes::FAILED)
    {
        std::cout << "FAILED DELETING" << std::endl;
    }
    else
    {
        std::cout << "UNKNOWN deleting message code" << std::endl;
    }
    qRegisterMetaType<Utility::DeletingMessageCodes>("Utility::DeletingMessageCodes");
    emit ReceiverManager::instance()->onUserMessageDeleteAnswer(deletingCode);
}

void ConnectionManager::onChannelLeaveAnswer(Utility::ChannelLeaveCodes ChannelLeaveCode)
{
    if (ChannelLeaveCode == Utility::ChannelLeaveCodes::SUCCESS)
    {
        std::cout << "SUCCESS LEAVING" << std::endl;
    }
    else if (ChannelLeaveCode == Utility::ChannelLeaveCodes::FAILED)
    {
        std::cout << "FAILED LEAVING" << std::endl;
    }
    else if (ChannelLeaveCode == Utility::ChannelLeaveCodes::CHANNEL_NOT_FOUND)
    {
        std::cout << "CHANNEL NOT FOUND" << std::endl;
    }
    qRegisterMetaType<Utility::ChannelLeaveCodes>("Utility::ChannelLeaveCodes");
    emit ReceiverManager::instance()->onChannelLeaveAnswer(ChannelLeaveCode);
}

void ConnectionManager::onChannelSubscribingAnswer(const Utility::ChannelSubscribingCodes subscribingChannelCode)
{
    if (subscribingChannelCode == Utility::ChannelSubscribingCodes::SUCCESS)
    {
        std::cout << "SUCCESS SUBSCRIBING" << std::endl;
    }
    else if (subscribingChannelCode == Utility::ChannelSubscribingCodes::FAILED)
    {
        std::cout << "FAILED SUBSCRIBING" << std::endl;
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
        std::cout << "SUCCESS DELETING" << std::endl;
    }
    else if (channelDeleteCode == Utility::ChannelDeleteCode::FAILED)
    {
        std::cout << "FAILED DELETING" << std::endl;
    }
    else if (channelDeleteCode == Utility::ChannelDeleteCode::CHANNEL_NOT_FOUND)
    {
        std::cout << "CHANNEL NOT FOUND" << std::endl;
    }
    else if (channelDeleteCode == Utility::ChannelDeleteCode::CHANNEL_IS_NOT_USER)
    {
        std::cout << "CHANNEL DOES NOT BELONG TO THE USER" << std::endl;
    }
    qRegisterMetaType<Utility::ChannelDeleteCode>("Utility::ChannelDeleteCode");
    emit ReceiverManager::instance()->onChannelDeleteAnswer(channelDeleteCode);
}

void ConnectionManager::onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode)
{
    if (channelCreateCode == Utility::ChannelCreateCodes::SUCCESS)
    {
        std::cout << "SUCCESS CREATING" << std::endl;
    }
    else if (channelCreateCode == Utility::ChannelCreateCodes::FAILED)
    {
        std::cout << "FAILD CREATING" << std::endl;
    }
    else if (channelCreateCode == Utility::ChannelCreateCodes::CHANNEL_ALREADY_CREATED)
    {
        std::cout << "CHANNEL ALREADY CREATE" << std::endl;
    }
    qRegisterMetaType<Utility::ChannelCreateCodes>("Utility::ChannelCreateCodes");
    emit ReceiverManager::instance()->onChannelCreateAnswer(channelCreateCode);
}

void ConnectionManager::onDirectMessageCreateAnswer(Utility::DirectMessageStatus directMessageCreateAnswer)
{
    if (directMessageCreateAnswer == Utility::DirectMessageStatus::SUCCESS)
    {
        std::cout << "SUCCESS CREATING" << std::endl;
    }
    else if (directMessageCreateAnswer == Utility::DirectMessageStatus::FAILED)
    {
        std::cout << "FAILED CREATING" << std::endl;
    }
    qRegisterMetaType<Utility::DirectMessageStatus>("Utility::DirectMessageStatus");
    emit ReceiverManager::instance()->onDirectMessageCreateAnswer(directMessageCreateAnswer);
}

void ConnectionManager::onDisconnect()
{
    disconnectFromServer();
    loginState = LoginState::FAILED;
    oApp->showMessage("Connection Status", "You've been disconnected from server!");
    emit ReceiverManager::instance()->onDisconnect();
    onReconnect();
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
