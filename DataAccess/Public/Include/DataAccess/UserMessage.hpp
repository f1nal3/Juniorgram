#pragma once
#include <Utility/WarningSuppression.hpp>
#include <chrono>
#include <cstring>
#include <string>

class UserMessage
{
    using time_point = std::chrono::time_point<std::chrono::system_clock>;

private:
    std::uint64_t userID;
    char messageText[256];
    time_point timestamp;

public:
    UserMessage() = default;

    UserMessage(const std::uint64_t userID, const std::string& messageText,
                   const time_point& timestamp)
        : userID(userID), timestamp(timestamp)
    {
        suppressWarning(4996, -Winit-self) 
            strcpy(this->messageText, messageText.data());
        restoreWarning
    }

    UserMessage(const UserMessage&) = default;

    std::uint64_t getUserID() const { return userID; }

    std::string getMessageText() const { return messageText; }

    time_point getTimestamp() const { return timestamp; }
};
