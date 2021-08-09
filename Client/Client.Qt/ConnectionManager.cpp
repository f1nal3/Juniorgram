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
    auto copy = channels;
    ChannelListWindow::setChannels(std::move(copy));

    ChannelListWindow::mainWidgetStatus.notify_one();
    emit ReceiverManager::instance()->onChannelListRequest(channels);
}

void ConnectionManager::onMessageHistoryAnswer(const std::vector<Network::MessageInfo>& messages)
{
    std::cout << "Message history received: \n";
    for (auto&& item : messages)
    {
        std::cout << item.message << '\n';
    }
    emit ReceiverManager::instance()->onMessageHistoryAnswer(messages);
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

void ConnectionManager::onUserMessageDeleteAnswer()
{
    Client::onUserMessageDeleteAnswer();
    emit ReceiverManager::instance()->onUserMessageDeleteAnswer();
}

void ConnectionManager::onDisconnect()
{
    disconnectFromServer();
    loginState = LoginState::FAILED;
    emit ReceiverManager::instance()->onDisconnect();
}
