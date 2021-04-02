#pragma once

#include <cstdint>
#include <string>

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
