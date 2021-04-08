#pragma once
#include <Network/Message.hpp>

struct MessageWrapper
{
    std::uint16_t userID;
    std::string messageText;
    std::chrono::time_point<std::chrono::system_clock> timestamp;

    MessageWrapper() = default;

    MessageWrapper(Network::Message& message)
    {
        MessageWrapper mw;
        message >> mw;

        userID      = mw.userID;
        messageText = mw.messageText;
        timestamp   = message.mHeader.mTimestamp;
    }
};
