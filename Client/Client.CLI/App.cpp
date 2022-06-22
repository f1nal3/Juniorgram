#include "App.hpp"

#include "ServerInfo.hpp"

App::App(/* args */) { connectToServer(ServerInfo::Address::remote, ServerInfo::Port::production); }

App::~App()
{
    if (isConnected())
    {
        disconnectFromServer();

        Base::Logger::FileLogger::getInstance().log("Server Down", Base::Logger::LogLevel::INFO);
    }
}

void App::onServerAccepted() 
{
    Base::Logger::FileLogger::getInstance().log("Server Accepted Connection", Base::Logger::LogLevel::INFO);
}

void App::onServerPing(double timestamp) 
{ 
    Base::Logger::FileLogger::getInstance().log
    (
        "Ping: " + std::to_string(timestamp), 
        Base::Logger::LogLevel::INFO
    );
}

void App::onServerMessage(const uint64_t clientId) 
{ 
    Base::Logger::FileLogger::getInstance().log
    (
        "Hello from [" + std::to_string(clientId) + "]", 
        Base::Logger::LogLevel::INFO
    );
}

void App::onChannelListRequest(const std::vector<Base::Models::ChannelInfo>& channels)
{
    for (const auto& item : channels)
    {
        std::ostringstream out;
        out << item.channelName;
        Base::Logger::FileLogger::getInstance().log(out.str(), Base::Logger::LogLevel::INFO);
    }
}

void App::onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode)
{
    if (storingMessageCode == Utility::StoringMessageCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log("SUCCESS sending", Base::Logger::LogLevel::INFO);
    }
    else if (storingMessageCode == Utility::StoringMessageCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log("FAILED sending", Base::Logger::LogLevel::ERR);
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log("Unknown StoringMessageCode", Base::Logger::LogLevel::WARNING);
    }
}

void App::onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState) 
{
    if(deletingState == Utility::DeletingMessageCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log("SUCCESS deleting", Base::Logger::LogLevel::INFO);
    }
    else if (deletingState == Utility::DeletingMessageCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log("FAILED deleting", Base::Logger::LogLevel::ERR);
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log("Unknown StoringMessageCode", Base::Logger::LogLevel::WARNING);
    }
}

void App::onMessageHistoryAnswer(const std::vector<Base::Models::MessageInfo>& messages)
{
    for (const auto& item : messages)
    {
        std::ostringstream out;
        out << item.message;
        Base::Logger::FileLogger::getInstance().log(out.str(), Base::Logger::LogLevel::INFO);
    }
}

void App::onRegistrationAnswer(Utility::RegistrationCodes registrationCode)
{
    if (registrationCode == Utility::RegistrationCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log("User was added", Base::Logger::LogLevel::INFO);
    }
    else if (registrationCode == Utility::RegistrationCodes::LOGIN_ALREADY_EXISTS)
    {
        Base::Logger::FileLogger::getInstance().log("User with such login already exists", Base::Logger::LogLevel::WARNING);
    }
    else if (registrationCode == Utility::RegistrationCodes::EMAIL_ALREADY_EXISTS)
    {
        Base::Logger::FileLogger::getInstance().log("User with such email already exists", Base::Logger::LogLevel::WARNING);
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log("Unknown RegistrationCode", Base::Logger::LogLevel::WARNING);
    }
}
