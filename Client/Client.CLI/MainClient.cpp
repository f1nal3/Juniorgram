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

                Network::MessageInfo msg;
                msg.userID  = 777;
                msg.message = "123";
                message.mBody = std::any_cast<Network::MessageInfo>(msg);

                clientApp.shell()->send(message);
                cmd = "";
            }
            else if (cmd == "qs")
            {
                Network::Message message;
                Network::MessageInfo msg;
                msg.userID  = 777;
                msg.message = "123";
                message.mBody = std::any_cast<Network::MessageInfo>(msg);

                clientApp.shell()->send(message);
            }
            else if (cmd == "cl")
            {
                clientApp.shell()->askForChannelList();
            }
            else if (cmd == "mh")
            {
                clientApp.shell()->askForMessageHistory();
            }
            else if (cmd == "sm")
            {
                std::vector<std::string> messagesList = {"hi, babe", "I'm comming today at 10 pm", "Expect"};
                clientApp.shell()->storeMessages(messagesList);
            }
            else if (cmd == "ur")
            {
                std::string email    = /*GetLineFromCin()*/ "1lessoct1o@main.ru";
                std::string login    = "gabrieeeeel111";
                std::string password = "qwerty124";
                
                clientApp.shell()->userRegistration(email, login, password);
            }
            else if (cmd == "ul")
            {
                std::cout << "To log in, please enter your email, login and password." << std::endl;

                std::string email    = /*GetLineFromCin()*/ "1lessoct1o@main.ru";
                std::string login    = "gabrieeeeel111";
                std::string password = "qwerty124";
            }
            else if (cmd == "q")
            {
               /* std::string accss =
                    "eyJhbGciOiJFQ0RTQS9FTVNBMShTSEEtMjU2KSIsInR5cCI6IkpXVCJ9.eyJleHAiOiIxNjI4NDY1OTYzIiwiaWF0IjoiMTYyODQ2MjM2MyIsImlkIjoiYWI1ODg0NDYtODVlMS00YzM5LTkyOTItN2I5ZWQ1NDFkY2U4IiwiaXAiOiIxMjcuMC4wLjEiLCJqdGkiOiJhY2Nlc3NfdG9rZW4iLCJvcyI6IldpbmRvd3MiLCJwcnQiOiJqZy5jbGllbnQiLCJzdWIiOiJSYjZoMkEzWGg0amlYbGt6SWoyU3FoMG5Sanc9IiwidXBkIjoiMTYyODQ2MjM2MyJ9."
                    "Ï";

                std::string refr = "eyJhbGciOiJFQ0RTQS9FTVNBMShTSEEtMjU2KSIsInR5cCI6IkpXVCJ9.eyJleHAiOiIxNjI4NTUxMjAzIiwiaWF0IjoiMTYyODQ2NDgwMyIsImlkIjoiODRiMzMwY2UtZjFlYS00ZjZhLTlmMzEtNjBiMWFhNDMwOGJjIiwiaXAiOiIxMjcuMC4wLjEiLCJqdGkiOiJyZWZyZXNoX3Rva2VuIiwib3MiOiJXaW5kb3dzIiwicHJ0IjoiamcuY2xpZW50Iiwic3ViIjoiUmI2aDJBM1hoNGppWGxreklqMlNxaDBuUmp3PSIsInVwZCI6IjE2Mjg0NjQ4MDMifQ==.õ1?¢Â³¼¯¾·=øSãv~jå;S×‘ô—©@aId.XÝµƒìê8ý·ˆÓ˜Ë{†œJ[XÞNè4";*/

                quit = true;
                clientApp.shell()->disconnect();
            }

            // Set a new line. Subtle race condition between the previous line
            // and this. Some lines could be missed. To aleviate, you need an
            // io-only thread. I'll give an example of that as well.
            future = std::async(std::launch::async, GetLineFromCin);
        }

        quit = clientApp.loop();
    }
    return EXIT_SUCCESS;
}
