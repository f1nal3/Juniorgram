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
    qRegisterMetaType<std::vector<Network::ChannelInfo> >("std::vector<Network::ChannelInfo>");
    emit ReceiverManager::instance()->onChannelListRequest(channels);
}

void ConnectionManager::onMessageHistoryAnswer(const std::vector<Network::MessageInfo>& messages)
{
    qRegisterMetaType<std::vector<Network::MessageInfo> >("std::vector<Network::MessageInfo>");
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

void ConnectionManager::onSubscribingChannelAnswer(const Utility::SubscribingChannelCodes subscribingChannelCode)
{
    if (subscribingChannelCode == Utility::SubscribingChannelCodes::SUCCESS)
    {
        std::cout << "SUCCESS SUBSCRIBING" << std::endl;
    }
    else if (subscribingChannelCode == Utility::SubscribingChannelCodes::FAILED)
    {
        std::cout << "FAILED SUBSCRIBING" << std::endl;
    }

    emit ReceiverManager::instance()->onSubscriptionChannelAnswer(subscribingChannelCode);
}

void ConnectionManager::onSubscribingChannelListAnswer(const std::vector<uint64_t> subscribingChannelList)
{
    //emit ReceiverManager::instance()->onSubscribingChannelListAnswer(subscribingChannelCode); commented out before the implementation of the function
}
void ConnectionManager::onDisconnect()
{
    disconnectFromServer();
    loginState = LoginState::FAILED;
    emit ReceiverManager::instance()->onDisconnect();
}
