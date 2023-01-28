#pragma once 

#include <DataAccess/IAdapter.hpp>
#include <DataAccess/IServerRepositories.hpp>

#include "MockQuery.hpp"
#include "MessageFiller.hpp"

namespace MockRepositories
{
using IAdapter                 = DataAccess::IAdapter;
using IChannelRepository       = DataAccess::IChannelsRepository;
using IDirectMessageRepository = DataAccess::IDirectMessageRepository;
using ILoginRepository         = DataAccess::ILoginRepository;
using IMessagesRepository      = DataAccess::IMessagesRepository;
using IRegisterRepository      = DataAccess::IRegisterRepository;
using IRepliesRepository       = DataAccess::IRepliesRepository;
using MessageFiller            = MesgFiller::MessageFiller;
using Query                    = MockQuery::MockQuery;

/**
* @brief class TestAbstractRepository.
* @details Designed to query a specific repository. /
*          The class contains two members: /
*          std::unique_ptr<Query> _pTable(for creating a query), /
*          MessageFiller _mesgFiller(for filling repository messages with data). 
*/
struct TestAbstractRepository
{
public:
    std::unique_ptr<Query>& getTable() { return _pTable; }
    MessageFiller&          getMessageFiller() { return _mesgFiller; }
private:
    std::unique_ptr<Query> _pTable;
    MessageFiller          _mesgFiller;
};

/**
* @brief class testChannelsRepository.
* @details Inherited from IChannelRepository class (for overriding methods related to this repository) \
*          and TestAbstractRepository(class to create a request to the repository).  
*/
struct testChannelsRepository final : IChannelRepository, TestAbstractRepository
{
    explicit testChannelsRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        getTable() = std::make_unique<Query>("users", adapter);
        getMessageFiller().fillChannelInfo();
        getMessageFiller().fillChannelLeaveInfo();
        getMessageFiller().fillChannelDeleteInfo();
        getMessageFiller().fillChannelSubscriptionInfo();
    }

    Utility::ChannelDeleteCode        deleteChannel(const Models::ChannelDeleteInfo& channel) override;
    Utility::ChannelCreateCodes       createChannel(const Models::ChannelInfo& channel) override;
    Utility::ChannelLeaveCodes        leaveChannel(const Models::ChannelLeaveInfo& channel) override;
    Utility::ChannelSubscribingCodes  subscribeToChannel(const Models::ChannelSubscriptionInfo& channel) override;
    std::vector<uint64_t>             getChannelSubscriptionList(const uint64_t userID) override;
    std::vector<Models::ChannelInfo>  getAllChannelsList() override;

    ~testChannelsRepository() override = default;
};

/**
* @brief class testDirectMessageRepository.
* @details Inherited from IDirectMessageRepository class (for overriding methods related to this repository) \
*          and TestAbstractRepository(class to create a request to the repository).  
*/
struct testDirectMessageRepository final : IDirectMessageRepository, TestAbstractRepository
{
    explicit testDirectMessageRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        getTable() = std::make_unique<Query>("channels", adapter);
        getMessageFiller().fillMessageInfo();
    }

    Utility::DirectMessageStatus addDirectChat(uint64_t userID, uint64_t receiverID) override;

    ~testDirectMessageRepository() override = default;
};

/**
* @brief class testLoginRepository.
* @details Inherited from ILoginRepository class (for overriding methods related to this repository) \
*          and TestAbstractRepository(class to create a request to the repository).  
*/
struct testLoginRepository final : ILoginRepository, TestAbstractRepository
{
    explicit testLoginRepository(const std::shared_ptr<IAdapter>& adapter)
    { 
        getTable() = std::make_unique<Query>("users", adapter); 
        getMessageFiller().fillLoginInfo();
    }

    std::uint64_t loginUser(const Models::LoginInfo& loginInfo) override;

    ~testLoginRepository() override = default;
};

/**
* @brief class testMessagesRepository.
* @details Inherited from IMessagesRepository class (for overriding methods related to this repository) \
*          and TestAbstractRepository(class to create a request to the repository).  
*/
struct testMessagesRepository final : IMessagesRepository, TestAbstractRepository
{
    explicit testMessagesRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        getTable() = std::make_unique<Query>("users", adapter);
        getMessageFiller().fillMessageInfo();
    }

    Utility::DeletingMessageCodes     deleteMessage(const Models::MessageInfo& messageInfo) override;
    Utility::ReactionMessageCodes     updateMessageReactions(const Models::MessageInfo& messageInfo) override;
    Utility::EditingMessageCodes      editMessage(const Models::MessageInfo& messageInfo) override;
    Utility::StoringMessageCodes      storeMessage(const Models::MessageInfo& messageInfo) override;
    std::vector<Models::MessageInfo>  getMessageHistory(const std::uint64_t channelID) override;

    ~testMessagesRepository() override = default;
};

/**
* @brief class testRegisterRepository.
* @details Inherited from IRegisterRepository class (for overriding methods related to this repository) \
*          and TestAbstractRepository(class to create a request to the repository).  
*/
struct testRegisterRepository final : IRegisterRepository, TestAbstractRepository
{
    explicit testRegisterRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        getTable() = std::make_unique<Query>("users", adapter);
        getMessageFiller().fillRegistrationInfo();
    }

    Utility::RegistrationCodes registerUser(const Models::RegistrationInfo& regInfo) override;

    ~testRegisterRepository() override = default;
};

/**
* @brief class testRepliesRepository.
* @details Inherited from IRepliesRepository class (for overriding methods related to this repository) \
*          and TestAbstractRepository(class to create a request to the repository).  
*/
struct testRepliesRepository final : IRepliesRepository, TestAbstractRepository
{
    explicit testRepliesRepository(const std::shared_ptr<IAdapter>& adapter)
    { 
        getTable() = std::make_unique<Query>("msgs", adapter); 
        getMessageFiller().fillReplyInfo();
    }

    std::vector<Models::ReplyInfo> getReplyHistory(const std::uint64_t channelID) override;
    Utility::StoringReplyCodes     storeReply(const Models::ReplyInfo& replyInfo) override;

    ~testRepliesRepository() override = default;
};
}  // namespace TestRepositories
