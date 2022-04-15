#include "App.hpp"

#include "ServerInfo.hpp"

App::App(/* args */) { connectToServer(ServerInfo::address, ServerInfo::port); }

App::~App()
{
    if (isConnected())
    {
        disconnectFromServer();
        std::cout << "Server Down\n";
    }
}

void App::onServerAccepted() { std::cout << "Server Accepted Connection\n"; }

void App::onServerPing(double timestamp) { std::cout << "Ping: " << timestamp << "\n"; }

void App::onServerMessage(const uint64_t clientId) { std::cout << "Hello from [" << clientId << "]\n"; }

void App::onChannelListRequest(const std::vector<Network::ChannelInfo>& channels)
{
    for (const auto& item : channels)
    {
        std::cout << item.channelName << '\n';
    }
}

void App::onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode)
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

void App::onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState) 
{
    if(deletingState == Utility::DeletingMessageCodes::SUCCESS)
    {
        std::cout << "SUCCESS deleting" << std::endl;
    }
    else if (deletingState == Utility::DeletingMessageCodes::FAILED)
    {
        std::cout << "FAILED deleting" << std::endl;
    }
    else
    {
        std::cout << "Unknown StoringMessageCode" << std::endl;
    }
}

void App::onMessageHistoryAnswer(const std::vector<Network::MessageInfo>& messages)
{
    for (const auto& item : messages)
    {
        std::cout << item.message << '\n';
    }
}

void App::onRegistrationAnswer(Utility::RegistrationCodes registrationCode)
{
    if (registrationCode == Utility::RegistrationCodes::SUCCESS)
    {
        std::cout << "User was added" << std::endl;
    }
    else if (registrationCode == Utility::RegistrationCodes::LOGIN_ALREADY_EXISTS)
    {
        std::cout << "User with such login already exists" << std::endl;
    }
    else if (registrationCode == Utility::RegistrationCodes::EMAIL_ALREADY_EXISTS)
    {
        std::cout << "User with such email already exists" << std::endl;
    }
    else
    {
        std::cout << "Unknown RegistrationCode" << std::endl;
    }
}
