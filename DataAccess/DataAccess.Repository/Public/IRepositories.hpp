#pragma once

#include <Network/Primitives.hpp>
#include <Utility/Utility.hpp>
#include <string>
#include <vector>

namespace DataAccess
{
struct IMasterRepository
{
    virtual ~IMasterRepository() = default;
};

struct IMessagesRepository : IMasterRepository
{
    virtual std::vector<Network::MessageInfo> getMessageHistoryForUser(const std::uint64_t channelID) = 0;

    virtual Utility::StoringMessageCodes storeMessage(const Network::MessageInfo& mi) = 0;

    virtual Utility::DeletingMessageCodes deleteMessage(const Network::MessageInfo& mi) = 0;

    virtual ~IMessagesRepository() = default;
};

struct IRepliesRepository : IMasterRepository
{
    virtual std::vector<Network::ReplyInfo> getReplyHistoryForUser(const std::uint64_t channelID) = 0;

    virtual ~IRepliesRepository() = default;
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

}  // namespace DataAccess
