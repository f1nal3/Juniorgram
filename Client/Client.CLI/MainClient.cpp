#include <chrono>
#include <future>
#include <iostream>
#include <string>

#include "App.hpp"
#include "Utility/UserDataValidation.hpp"

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
                const std::uint64_t channelID = 2;
                clientApp.shell()->askForMessageHistory(channelID);
            }
            else if (cmd == "sm")
            {
                std::string text = GetLineFromCin();
                uint64_t userID     = 99;
                uint64_t channelID  = 1;

                clientApp.shell()->storeMessage(text, userID, channelID);
            }
            else if (cmd == "ur")
            {
                std::string email = GetLineFromCin();
                std::string login = GetLineFromCin();
                std::string password = GetLineFromCin();
                std::string repeatPassword = GetLineFromCin();
                
                if (email.empty() || login.empty() || password.empty() || repeatPassword.empty())
                {
                    std::cout << "some field is empty" << std::endl;
                    
                    future = std::async(std::launch::async, GetLineFromCin);
                    continue;
                }

                if (password != repeatPassword)
                {
                    std::cout << "passwords are different" << std::endl;
                    
                    future = std::async(std::launch::async, GetLineFromCin);
                    continue;
                }

                using namespace UserDataValidation;
                if (!isLoginValid(login))
                {
                    std::cout << "login is not valid" << std::endl;
                    
                    future = std::async(std::launch::async, GetLineFromCin);
                    continue;
                }

                if (!isEmailValid(email))
                {
                    std::cout << "email is not valid" << std::endl;
                    
                    future = std::async(std::launch::async, GetLineFromCin);
                    continue;
                }

                if (!isPasswordValid(password))
                {
                    std::cout << "password is not valid" << std::endl;
                    
                    future = std::async(std::launch::async, GetLineFromCin);
                    continue;
                }

                clientApp.shell()->userRegistration(email, login, password);
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
