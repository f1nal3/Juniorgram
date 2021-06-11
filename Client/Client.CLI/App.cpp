#include "App.hpp"
#include "Utility/YasSerializer.hpp"
#include "Network/Primitives.hpp"

#include <DataAccess.Static/SQLCipherAdapter.hpp>

App::App(/* args */)
{
    if (client.isRefreshTokenExists())
    {
        client.connect(address, port);
        client.askForChannelList();
    }
    else
    {
        
    }
}
    

App::~App()
{
    if (client.isConnected())
    {
        client.disconnect();
        std::cout << "Server Down\n";
    }
}

Network::Client* App::shell() { return &client; }

bool App::loop()
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
                              << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
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
                    std::cout << "Channel list received: \n";
                    std::vector<std::string> channelList;
                    
                    for (const auto& item :
                         std::any_cast<std::vector<Network::ChannelInfo>>(message.mBody))
                    {
                        channelList.emplace_back(item.channelName);
                        std::cout << item.channelName << '\n';
                    }
                }
                break;

                case Network::Message::MessageType::MessageHistoryRequest:
                {
                    std::cout << "Message history received: \n";
                    std::vector<std::string> messageList;

                    for (const auto& item :
                         std::any_cast<std::vector<Network::MessageInfo>>(message.mBody))
                    {
                        messageList.emplace_back(item.message);
                        std::cout << item.message << '\n';
                    }
                }
                break;

                case Network::Message::MessageType::MessageStoreRequest:
                {
                    std::cout << "Message were stored" << std::endl;
                }
                break;

                case Network::Message::MessageType::RegistrationAnswer:
                {
                    auto code = std::any_cast<Utility::RegistrationCodes>(message.mBody);

                    if (code == Utility::RegistrationCodes::SUCCESS)
                    {
                        std::cout << "User was added" << std::endl;
                    } 
                    else if (code == Utility::RegistrationCodes::LOGIN_ALREADY_EXISTS)
                    {
                        std::cout << "User with such login already exists" << std::endl;
                    }
                    else if (code == Utility::RegistrationCodes::EMAIL_ALREADY_EXISTS)
                    {
                        std::cout << "User with such email already exists" << std::endl;
                    }
                    else
                    {
                        std::cout << "Unknown RegistrationCode" << std::endl;
                    }
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
        std::cout << "Server Down\n";
        return true;
    }
    return false;
}
