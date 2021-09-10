#pragma once

#include <Network/Primitives.hpp>
#include <string>
#include <vector>
#include <Utility/Utility.hpp>

namespace DataAccess
{

struct IMasterRepository
{
    virtual ~IMasterRepository() = default;
};

struct IMessagesRepository : IMasterRepository
{
    virtual std::vector<Network::MessageInfo>   getMessageHistoryForUser(const std::uint64_t channelID) = 0;

    virtual Utility::StoringMessageCodes storeMessage(const Network::MessageInfo& mi) = 0;

    virtual Utility::DeletingMessageCodes deleteMessage(const Network::MessageInfo& mi) = 0;

    virtual ~IMessagesRepository() = default;
};

struct IChannelsRepository : IMasterRepository
{
    virtual std::vector<Network::ChannelInfo> getAllChannelsList() = 0;

    virtual ~IChannelsRepository() = default;
};

struct IRegisterRepository : IMasterRepository
{
    virtual Utility::RegistrationCodes registerUser(const Network::RegistrationInfo& ri) = 0;

    virtual ~IRegisterRepository() = default;
};

struct ILoginRepository : IMasterRepository
{
    virtual std::uint64_t loginUser(const std::string& login, std::string& pwdHash) = 0;

    virtual ~ILoginRepository() = default;
};

}
