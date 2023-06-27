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

    App  clientApp;

    while (!clientApp.isConnected());

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
                clientApp.pingKernel();
                cmd = "";
            }
            else if (cmd == "s")
            {
                Network::Message message;                                  

                Models::MessageInfo msg;
                msg._message   = "123";
                message.mBody = std::any_cast<Models::MessageInfo>(msg);

                clientApp.send(message);
                cmd = "";
            }
            else if (cmd == "qs")
            {
                Network::Message message;

                Models::MessageInfo msg;
                msg._message   = "123";
                message.mBody = std::any_cast<Models::MessageInfo>(msg);

                clientApp.send(message);
            }
            else if (cmd == "cl")
            {
                clientApp.askForChannelList();
            }
            else if (cmd == "mh")
            {
                const std::uint64_t channelID = 2;
                clientApp.askForMessageHistory(channelID);
            }
            else if (cmd == "sm")
            {
                std::string text      = GetLineFromCin();
                uint64_t    channelID = 1;

                clientApp.storeMessage(text, channelID);
            }
            else if (cmd == "dm")
            {
                std::uint64_t messageID;
                std::cin >> messageID;

                clientApp.userMessageDelete(messageID);
            }
            else if (cmd == "ur")
            {
                std::string email          = GetLineFromCin();
                std::string login          = GetLineFromCin();
                std::string password       = GetLineFromCin();
                std::string repeatPassword = GetLineFromCin();

                if (email.empty() || login.empty() || password.empty() || repeatPassword.empty())
                {
                    Base::Logger::FileLogger::getInstance().log("some field is empty", Base::Logger::LogLevel::WARNING);

                    future = std::async(std::launch::async, GetLineFromCin);
                    continue;
                }

                if (password != repeatPassword)
                {
                    Base::Logger::FileLogger::getInstance().log("passwords are different", Base::Logger::LogLevel::WARNING);


                    future = std::async(std::launch::async, GetLineFromCin);
                    continue;
                }

                using namespace UserDataValidation;
                if (!isLoginValid(login))
                {
                    Base::Logger::FileLogger::getInstance().log("login is not valid", Base::Logger::LogLevel::WARNING);


                    future = std::async(std::launch::async, GetLineFromCin);
                    continue;
                }

                if (!isEmailValid(email))
                {
                    Base::Logger::FileLogger::getInstance().log("email is not valid", Base::Logger::LogLevel::WARNING);


                    future = std::async(std::launch::async, GetLineFromCin);
                    continue;
                }

                if (!isPasswordValid(password))
                {
                    Base::Logger::FileLogger::getInstance().log("password is not valid", Base::Logger::LogLevel::WARNING);


                    future = std::async(std::launch::async, GetLineFromCin);
                    continue;
                }

                clientApp.userRegistration(email, login, password);
            }
            else if (cmd == "q")
            {
                quit = true;
                clientApp.disconnectFromKernel();
                continue;
            }
            // Set a new line. Subtle race condition between the previous line
            // and this. Some lines could be missed. To aleviate, you need an
            // io-only thread. I'll give an example of that as well.
            future = std::async(std::launch::async, GetLineFromCin);
        }
    }

    return EXIT_SUCCESS;
}
