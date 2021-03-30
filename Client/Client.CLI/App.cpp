#include "App.hpp"
#include "Network/Primitives.hpp"

App::App(/* args */) { client.connect(address, port); }

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

            switch (message.mHeader.mID)
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
                              << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
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

                    for(auto& item : channelList)
                        std::cout << item << '\n';
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
