#include "App.hpp"

App::App(/* args */) { client.connect(address, port); }

App::~App()
{
    if (client.isConnected())
    {
        client.disconnect();
        std::cout << "Server Down\n";
    }
}

network::Client* App::shell() { return &client; }

bool App::loop()
{
    if (client.isConnected())
    {
        if (!client.incoming().empty())
        {
            network::Message message = client.incoming().pop_front();

            switch (message.mHeader.mID)
            {
                case network::Message::MessageType::ServerAccept:
                {
                    std::cout << "Server Accepted Connection\n";
                }
                break;

                case network::Message::MessageType::ServerPing:
                {
                    std::chrono::system_clock::time_point timeNow =
                        std::chrono::system_clock::now();
                    std::chrono::system_clock::time_point timeThen;
                    message >> timeThen;
                    std::cout << "Ping: "
                              << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
                }
                break;

                case network::Message::MessageType::ServerMessage:
                {
                    uint64_t clientID;
                    message >> clientID;
                    std::cout << "Hello from [" << clientID << "]\n";
                }
                break;

                case network::Message::MessageType::ChannelListRequest:
                {
                    std::cout << "Channel list recieved: " << message;
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
