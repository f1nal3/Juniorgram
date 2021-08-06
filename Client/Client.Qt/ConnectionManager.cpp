#include "ConnectionManager.hpp"

ReceiverManager* ReceiverManager::self;

ReceiverManager::ReceiverManager() { self = this; }

void ConnectionManager::onServerAccepted() { std::cout << "Server Accepted Connection\n"; }

void ConnectionManager::onServerPing(double timestamp) { std::cout << "Ping: " << timestamp << "\n"; }

void ConnectionManager::onServerMessage(const uint64_t clientId) { std::cout << "Hello from [" << clientId << "]\n"; }

void ConnectionManager::onChannelListRequest(const std::vector<Network::ChannelInfo>& channels)
{
    auto copy = channels;
    ChannelListWindow::setChannels(std::move(copy));

    ChannelListWindow::mainWidgetStatus.notify_one();
}

void ConnectionManager::onMessageHistoryAnswer(const std::vector<Network::MessageInfo>& messages)
{
    std::cout << "Message history received: \n";
    for (auto&& item : messages)
    {
        std::cout << item.message << '\n';
    }
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
    else
    {
        std::cout << "Unknown StoringMessageCode" << std::endl;
    }
}

void ConnectionManager::onRegistrationAnswer(Utility::RegistrationCodes registrationCode)
{
    if (registrationCode == Utility::RegistrationCodes::SUCCESS)
    {
        std::cout << "New user was registered!" << std::endl;
    }
    else if (registrationCode == Utility::RegistrationCodes::EMAIL_ALREADY_EXISTS)
    {
        std::cout << "Email already exists" << std::endl;
    }
    else if (registrationCode == Utility::RegistrationCodes::LOGIN_ALREADY_EXISTS)
    {
        std::cout << "Login already exists" << std::endl;
    }
    else
    {
        std::cout << "Unknown code" << std::endl;
    }
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

void ConnectionManager::onUserMessageDeleteAnswer() { Client::onUserMessageDeleteAnswer(); }

void ConnectionManager::onDisconnect()
{
    disconnectFromServer();
    loginState = LoginState::FAILED;
    emit ReceiverManager::instance()->onDisconnect();
}
