#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <queue>

#include "App.hpp"
#include "Utility/UserDataValidation.hpp"

static std::queue<std::string> queriesQueue;

std::string GetLineFromCin()
{
    std::string line;
    if (queriesQueue.empty())
    {
        std::getline(std::cin, line);

        queriesQueue.push(std::move(line));
    }

    line = queriesQueue.front();
    queriesQueue.pop();

    return line;
}

int main(int argc, char** argv)
{
    for (std::size_t argIndex = 1; argIndex < static_cast<std::size_t>(argc); argIndex++)
    {
        queriesQueue.emplace(argv[argIndex]);
    }

    queriesQueue.emplace("cl");

    App clientApp;
    
    // Without this string Performance test or Args fall client.
    // We need some kind of mechanism that would put
    // the thread to sleep until the client connects to the client.
    // 
    // This doesn't work.
    // while (!clientApp.shell()->isConnected());
    std::this_thread::sleep_for(std::chrono::seconds(5));

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
            // What is the diference between:
            else if (cmd == "s")                                           // <---- This
            {
                Network::Message message;                                  

                Network::MessageInfo msg;
                msg.userID  = 777;
                msg.message = "123";
                message.mBody = std::any_cast<Network::MessageInfo>(msg);

                clientApp.shell()->send(message);
                cmd = "";
            }
            else if (cmd == "qs")                                          // <---- And this?
            {
                Network::Message message;

                Network::MessageInfo msg;
                msg.userID  = 777;
                msg.message = "123";
                message.mBody = std::any_cast<Network::MessageInfo>(msg);

                clientApp.shell()->send(message);
            }
            // ========================================================
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
