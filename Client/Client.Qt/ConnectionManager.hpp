#pragma once

#include "Client.hpp"
#include "Network/Primitives.hpp"
#include "Widgets/ChannelListWindow.hpp"
#include "ServerInfo.hpp"

enum class LoginState
{
    SUCCESS,
    IN_PROGRESS,
    FAILED
};

class ConnectionManager
{
private:
    inline static Network::Client client;

public:
    
    inline static LoginState loginState;
    
    static void disconnect()
    {
        if (client.isConnected())
        {
            client.disconnect();
            loginState = LoginState::FAILED;
        }
    }

    static void connect() { client.connect(ServerInfo::address, ServerInfo::port); }

    static Network::Client& getClient() { return client; }

    static bool isConnected() { return client.isConnected(); }

    static void loop()
    {
        while (1)
        {
            if (client.isConnected())
            {
                if (!client.incoming().empty())
                {
                    Network::Message message = client.incoming().pop_front();

                    switch (message.mHeader.mMessageType)
                    {
                        case Network::Message::MessageType::ServerAccept:
                        {
                            std::cout << "Server Accepted Connection\n";
                        }
                        break;

                        case Network::Message::MessageType::ServerPing:
                        {
                            std::chrono::system_clock::time_point timeNow =
                                std::chrono::system_clock::now();
                            std::chrono::system_clock::time_point timeThen;
                            timeThen = message.mHeader.mTimestamp;
                            std::cout << "Ping: "
                                      << std::chrono::duration<double>(timeNow - timeThen).count()
                                      << "\n";
                        }
                        break;

                        case Network::Message::MessageType::ServerMessage:
                        {
                            // need a structure / variable that will contain client Id
                            uint64_t clientID = 0;
                            // message >> clientID;
                            std::cout << "Hello from [" << clientID << "]\n";
                        }
                        break;

                        case Network::Message::MessageType::ChannelListRequest:
                        {
                            auto channels = std::any_cast<std::vector<Network::ChannelInfo>>(message.mBody);
                            ChannelListWindow::setChannels(std::move(channels));
                            
                            ChannelListWindow::mainWidgetStatus.notify_one();
                        }
                        break;

                        case Network::Message::MessageType::MessageHistoryAnswer:
                        {
                            std::cout << "Message history received: \n";
                            std::vector<Network::MessageInfo> messageList;

                            for (auto&& item : std::any_cast<std::vector<Network::MessageInfo>>(message.mBody))
                            {
                                messageList.emplace_back(item);
                                std::cout << item.message << '\n';
                            }
                        }
                        break;

                        case Network::Message::MessageType::MessageStoreAnswer:
                        {
                            auto code = std::any_cast<Utility::StoringMessageCodes>(message.mBody);

                            if (code == Utility::StoringMessageCodes::SUCCESS)
                            {
                                std::cout << "SUCCESS sending" << std::endl;
                            }
                            else if (code == Utility::StoringMessageCodes::FAILED)
                            {
                                std::cout << "FAILED sending" << std::endl;
                            }
                            else
                            {
                                std::cout << "Unknown StoringMessageCode" << std::endl;
                            }
                        }
                        break;

                        case Network::Message::MessageType::RegistrationAnswer:
                        {
                            auto code = std::any_cast<Utility::RegistrationCodes>(message.mBody);

                            if (code == Utility::RegistrationCodes::SUCCESS)
                            {
                                std::cout << "New user was registered!" << std::endl;
                            }
                            else if (code == Utility::RegistrationCodes::EMAIL_ALREADY_EXISTS)
                            {
                                std::cout << "Email already exists" << std::endl;
                            }
                            else if (code == Utility::RegistrationCodes::LOGIN_ALREADY_EXISTS)
                            {
                                std::cout << "Login already exists" << std::endl;
                            }
                            else
                            {
                                std::cout << "Unknown code" << std::endl;
                            }
                        }
                        break;
                        
                        case Network::Message::MessageType::LoginAnswer:
                        {
                            auto loginSuccessful = std::any_cast<bool>(message.mBody);
                            if (loginSuccessful)
                            {
                                loginState = LoginState::SUCCESS;
                            }
                            else
                            {
                                loginState = LoginState::FAILED;
                            }
                        }
                        break;

                        case Network::Message::MessageType::UserMessageDeleteAnswer:
                        {
                            //Temporarily commented out code
                            //auto messageInfo = std::any_cast<Network::MessageDeletedInfo>(message.mBody);
                        }
                        break;

                        default:
                            break;
                    }
                }
            }
            else
            {
                client.disconnect();
                loginState = LoginState::FAILED;
                return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
};
