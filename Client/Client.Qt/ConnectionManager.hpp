#pragma once
#include "Client.hpp"
#include "Network/Primitives.hpp"
#include "Widgets/ChannelListWindow.hpp"

class ConnectionManager
{
public:
    static void disconnect()
    {
        if (client.isConnected())
        {
            client.disconnect();
            std::cout << "Server Down\n";
        }
    }

    static void connect() { client.connect(address, port); }

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
                            std::cout << "Channel list received: \n";
                            std::vector<std::string> channelList;

                            for (const auto& item :
                                 std::any_cast<std::vector<Network::ChannelInfo>>(message.mBody))
                            {
                                channelList.emplace_back(item.channelName);
                                std::cout << item.channelName << '\n';
                                ChannelListWindow::addChannelInfo(item.channelName);
                            }
                            ChannelListWindow::mainWidgetStatus.notify_one();
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

                        default:
                            break;
                    }
                }
            }
            else
            {
                client.disconnect();
                std::cout << "Server Down\n";
                return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

private:
    inline static Network::Client     client;
    inline static const std::string   address = "104.40.239.183";
    inline static const std::uint16_t port    = 65001;
};
