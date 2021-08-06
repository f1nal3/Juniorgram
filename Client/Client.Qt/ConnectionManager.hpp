#pragma once

#include "Client.hpp"
#include "Network/Primitives.hpp"
#include "ServerInfo.hpp"
#include "Widgets/ChannelListWindow.hpp"

enum class LoginState
{
    SUCCESS,
    IN_PROGRESS,
    FAILED
};

class ConnectionManager : public Network::Client
{
public:
    inline static LoginState loginState;

    void init() { connect(ServerInfo::address, ServerInfo::port); }

    void onServerAccepted() override { std::cout << "Server Accepted Connection\n"; }

    void onServerPing(double timestamp) override { std::cout << "Ping: " << timestamp << "\n"; }

    void onServerMessage(const uint64_t clientId) override { std::cout << "Hello from [" << clientId << "]\n"; }

    void onChannelListRequest(const std::vector<Network::ChannelInfo>& channels) override
    {
        auto copy = channels;
        ChannelListWindow::setChannels(std::move(copy));

        ChannelListWindow::mainWidgetStatus.notify_one();
    }

    void onMessageHistoryAnswer(const std::vector<Network::MessageInfo>& messages) override
    {
        std::cout << "Message history received: \n";
        for (auto&& item : messages)
        {
            std::cout << item.message << '\n';
        }
    }

    void onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode) override
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

    void onRegistrationAnswer(Utility::RegistrationCodes registrationCode) override
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

    void onUserMessageDeleteAnswer() override { Client::onUserMessageDeleteAnswer(); }

    void onLoginAnswer(bool success) override
    {
        if (success)
        {
            loginState = LoginState::SUCCESS;
        }
        else
        {
            loginState = LoginState::FAILED;
        }
    }

    void onDisconnect() override
    {
        disconnect();
        loginState = LoginState::FAILED;
    }
};
