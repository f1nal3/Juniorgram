#pragma once
#include <Client.hpp>
#include <Network/Primitives.hpp>

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

                    switch (message.mHeader.mConnectionID)
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
                            message >> timeThen;
                            std::cout << "Ping: "
                                      << std::chrono::duration<double>(timeNow - timeThen).count()
                                      << "\n";
                        }
                        break;

                        case Network::Message::MessageType::ServerMessage:
                        {
                            uint64_t clientID;
                            message >> clientID;
                            std::cout << "Hello from [" << clientID << "]\n";
                        }
                        break;

                        case Network::Message::MessageType::ChannelListRequest:
                        {
                            std::cout << "Channel list received: \n";
                            std::vector<std::string> channelList;

                            std::size_t channelListSize;
                            message >> channelListSize;

                            for (std::size_t i = 0; i < channelListSize; i++)
                            {
                                Network::ChannelInfo info;
                                message >> info;
                                channelList.emplace_back(info.channelName);
                            }

                            for (auto& item : channelList)
                            {
                                std::cout << item << '\n';
                                ChannelListWindow::addChannelInfo(item);
                            }
                            ChannelListWindow::mainWidgetStatus.notify_one();
                        }
                        break;

                        case Network::Message::MessageType::MessageHistoryRequest:
                        {
                            std::cout << "Message history received: \n";
                            std::vector<std::string> messageList;

                            std::size_t messageListSize;
                            message >> messageListSize;

                            for (std::size_t i = 0; i < messageListSize; i++)
                            {
                                Network::MessageInfo info;
                                message >> info;
                                messageList.emplace_back(std::string(info.message));
                            }

                            for (auto& item : messageList) std::cout << item << '\n';
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
