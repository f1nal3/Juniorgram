#include <network.h>

#include <chrono>
#include <future>
#include <iostream>
#include <string>

std::string GetLineFromCin() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

int main() {
    network::Client client;
    client.connect("127.0.0.1", 60000);

    bool quit   = false;
    auto future = std::async(std::launch::async, GetLineFromCin);

    while (!quit)
    {
        if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
        {
            static std::string cmd;
            cmd = future.get();
            if (cmd == "p")
            {
                client.pingServer();
                cmd = "";
            }
            else if (cmd == "m")
            {
                client.messageAll();
                cmd = "";
            }
            else if (cmd == "s")
            {
                network::Message message;
                message << "{'msg':123,'id':777}";
                client.send(message);
                cmd = "";
            }
            else if (cmd == "qs")
            {
                network::Message message;
                message << "{'msg':0,'id':0,'cmd':True}";
                client.send(message);
            }
            else if (cmd == "q")
            {
                quit = true;
                client.disconnect();
                continue;
            }
            // Set a new line. Subtle race condition between the previous line
            // and this. Some lines could be missed. To aleviate, you need an
            // io-only thread. I'll give an example of that as well.
            future = std::async(std::launch::async, GetLineFromCin);
        }

        // std::this_thread::sleep_for(std::chrono::seconds(1));

        if (client.isConnected())
        {
            if (!client.incoming().empty())
            {
                network::Message message = client.incoming().pop_front();

                switch (message.mHeader.mID)
                {
                    case network::Message::MessageType::ServerAccept: {
                        std::cout << "Server Accepted Connection\n";
                    }
                    break;

                    case network::Message::MessageType::ServerPing: {
                        std::chrono::system_clock::time_point timeNow =
                            std::chrono::system_clock::now();
                        std::chrono::system_clock::time_point timeThen;
                        message >> timeThen;
                        std::cout << "Ping: "
                                  << std::chrono::duration<double>(timeNow - timeThen).count()
                                  << "\n";
                    }
                    break;

                    case network::Message::MessageType::ServerMessage: {
                        uint64_t clientID;
                        message >> clientID;
                        std::cout << "Hello from [" << clientID << "]\n";
                    }
                    break;
                }
            }
        }
        else
        {
            client.disconnect();
            std::cout << "Server Down\n";
            quit = true;
        }
    }

    return 0;
}
