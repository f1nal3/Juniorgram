#include "ConnectionManager.hpp"

#include "Application.hpp"

ReceiverManager* ReceiverManager::self;

ReceiverManager::ReceiverManager() { self = this; }

void ConnectionManager::onServerAccepted()
{
    std::cout << "Server Accepted Connection\n";
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
    {
        std::cout << "SUCCESS sending" << std::endl;
    }
    else if (storingMessageCode == Utility::StoringMessageCodes::FAILED)
    {
        std::cout << "FAILED sending" << std::endl;
    }
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
        std::cout << "FAILD LEAVING" << std::endl;
    }
    else if(ChannelLeaveCode == Utility::ChannelLeaveCodes::CHANNEL_NOT_FOUND)
    {
        std::cout << "CHANNEL NOT FOUND" << std::endl;
    }
    qRegisterMetaType<Utility::ChannelLeaveCodes>("Utility::ChannelLeaveCodes");
    emit ReceiverManager::instance()->onChannelLeaveAnswer(ChannelLeaveCode);

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
    else if (channelDeleteCode == Utility::ChannelDeleteCode::SUCCESS)
    {
        std::cout << "FAILD DELETING" << std::endl;
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
    else if(channelCreateCode == Utility::ChannelCreateCodes::CHANNEL_ALREADY_CREATED)
    {
        std::cout << "CHANNEL ALREADY CREATE" << std::endl;
    }
    qRegisterMetaType<Utility::ChannelCreateCodes>("Utility::ChannelCreateCodes");
    emit ReceiverManager::instance()->onChannelCreateAnswer(channelCreateCode);
}

void ConnectionManager::onDisconnect()
{
    disconnectFromServer();
    loginState = LoginState::FAILED;
    emit ReceiverManager::instance()->onDisconnect();
}
