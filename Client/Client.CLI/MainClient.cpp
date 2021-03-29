#include <chrono>
#include <future>
#include <iostream>
#include <string>

#include "App.hpp"

std::string GetLineFromCin()
{
    std::string line;
    std::getline(std::cin, line);
    return line;
}

int main()
{
    App clientApp;
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
                clientApp.shell()->pingServer();
                cmd = "";
            }
            else if (cmd == "s")
            {
                Network::Message message;
                message << "{'msg':123,'id':777}";
                clientApp.shell()->send(message);
                cmd = "";
            }
            else if (cmd == "qs")
            {
                Network::Message message;
                message << "{'msg':0,'id':0,'cmd':True}";
                clientApp.shell()->send(message);
            }
            else if (cmd == "cl")
            {
                clientApp.shell()->askForChannelList();
            }
            else if (cmd == "q")
            {
                quit = true;
                clientApp.shell()->disconnect();
                continue;
            }
            // Set a new line. Subtle race condition between the previous line
            // and this. Some lines could be missed. To aleviate, you need an
            // io-only thread. I'll give an example of that as well.
            future = std::async(std::launch::async, GetLineFromCin);
        }

        // std::this_thread::sleep_for(std::chrono::seconds(1));

        quit = clientApp.loop();
    }
    return EXIT_SUCCESS;
}
