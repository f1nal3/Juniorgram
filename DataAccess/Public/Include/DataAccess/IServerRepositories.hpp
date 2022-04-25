#pragma once
#include <string>
#include <vector>

#include <Network/Primitives.hpp>
#include <Utility/Utility.hpp>

#include "IMasterRepository.hpp"

namespace DataAccess
{
/**
 * @brief The IMessagesRepository abstract subclass from IMasterRepository
 */
struct IMessagesRepository : IMasterRepository
{
    /// Pure Virtual method to get Message History
    virtual std::vector<Network::MessageInfo> getMessageHistory(const std::uint64_t channelID) = 0;

    /// Pure Virtual method to store Message
    virtual Utility::StoringMessageCodes storeMessage(const Network::MessageInfo& mi) = 0;

    /// Pure Virtual method to delete Message
    virtual Utility::DeletingMessageCodes deleteMessage(const Network::MessageInfo& mi) = 0;

    /// Pure Virtual method for editing Message
    virtual Utility::EditingMessageCodes editMessage(const Network::MessageInfo& mi) = 0;
    
    /// Pure Virtual method for updating message reactions
    virtual Utility::ReactionMessageCodes updateMessageReactions(const Network::MessageInfo& mi) = 0;

    /// Default virtual destructor
    virtual ~IMessagesRepository() = default;
};

/**
 * @brief The IRepliesRepository abstract subclass from IMasterRepository
 */
struct IRepliesRepository : IMasterRepository
{
    /// Virtual method to get Reply History
    virtual std::vector<Network::ReplyInfo> getReplyHistory(const std::uint64_t channelID) = 0;

    /// Virtual method to store Reply message
    virtual Utility::StoringReplyCodes storeReply(const Network::ReplyInfo& rsi) = 0;

    /// Default virtual destructor
    virtual ~IRepliesRepository() = default;
};

/**
 * @brief The IChannelsRepository abstract subclass from IMasterRepository
 */
struct IChannelsRepository : IMasterRepository
{
    /// Virtual method to get channel List
    virtual std::vector<Network::ChannelInfo> getAllChannelsList()                              = 0;
    /// Virtual method to delete channel
    virtual Utility::ChannelDeleteCode deleteChannel(const Network::ChannelDeleteInfo& channel) = 0;

    /// Virtual method to create channel
    virtual Utility::ChannelCreateCodes createChannel(const Network::ChannelInfo& channel) = 0;

    /// Virtual method to leave channel
    virtual Utility::ChannelLeaveCodes leaveChannel(const Network::ChannelLeaveInfo& channel)                    = 0;
    /// Virtual method for channel subscription
    virtual Utility::ChannelSubscribingCodes subscribeToChannel(const Network::ChannelSubscriptionInfo& channel) = 0;
    /// Virtual method to get list of subscribed channels
    virtual std::vector<uint64_t> getChannelSubscriptionList(uint64_t userID) = 0;

    /// Default virtual destructor
    virtual ~IChannelsRepository() = default;
};

/**
 * @brief The IChannelsRepository abstract subclass from IMasterRepository
 */
struct IDirectMessageRepository : IMasterRepository
{
    /**
     * @brief save a direct message from *user_id* to *receiverId*
     * @param user_id Sender id
     * @param directMessageInfo receiver info
     */
    virtual Utility::DirectMessageStatus addDirectChat(uint64_t user_id, uint64_t directMessageInfo) = 0;
};

/**
 * @brief The IRegisterRepository abstract subclass from IMasterRepository
 */
struct IRegisterRepository : IMasterRepository
{
    /// Virtual method to register User
    virtual Utility::RegistrationCodes registerUser(const Network::RegistrationInfo& ri) = 0;

    /// Default virtual destructor
    ~IRegisterRepository() override = default;
};

/**
 * @brief The ILoginRepository abstract subclass from IMasterRepository
 */
struct ILoginRepository : IMasterRepository
{
    /// Virtual method for login User
    virtual std::uint64_t loginUser(const Network::LoginInfo& li) = 0;

    /// Default virtual destructor
    virtual ~ILoginRepository() = default;
};

}  // namespace DataAccess
