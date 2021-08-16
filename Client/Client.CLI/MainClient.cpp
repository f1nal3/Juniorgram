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
                std::string email    = /*GetLineFromCin()*/ "1lessoct1o@mail.ru";
                std::string login    = "gabrieeeeel111";
                std::string password = "qwerty124";
                
                clientApp.shell()->userRegistration(email, login, password);
            }
            else if (cmd == "ul")
            {
                std::cout << "To log in, please enter your email, login and password." << std::endl;

                std::string email    = /*GetLineFromCin()*/ "1lessoct1o@mail.ru";
                std::string login    = "gabrieeeeel111";
                std::string password = "qwerty124";
            }
            else if (cmd == "q")
            {
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
