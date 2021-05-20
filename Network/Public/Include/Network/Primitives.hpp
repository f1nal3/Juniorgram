#pragma once

#include <Utility/WarningSuppression.hpp>
#include <cstdint>
#include <string>
#include <chrono>
#include <cstring>
#include <string>

namespace Network
{
    struct ChannelInfo
    {
        std::uint64_t channelID;
        std::string channelName;
    };

    template <typename Archive>
    void serialize(Archive& ar, Network::ChannelInfo& o)
    {
        ar& o.channelID& o.channelName;
    }
    
    struct MessageInfo
    {
        std::uint64_t userID;
        std::string message;
    };

    template <typename Archive>
    void serialize(Archive& ar, Network::MessageInfo& o)
    {
        ar& o.userID& o.message;
    }
    } // namespace Network
