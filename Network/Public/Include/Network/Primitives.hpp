#pragma once

#include <Utility/WarningSuppression.hpp>
#include <cstdint>
#include <string>
#include <chrono>
#include <cstring>

namespace Network
{
    struct ChannelInfo
    {
        std::uint64_t channelID;
        char channelName[256];
    };
    
    struct MessageInfo
    {
        std::uint64_t userID;
        char message[256];
    };
} // namespace Network
