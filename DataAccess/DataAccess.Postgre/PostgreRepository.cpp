#include "PostgreRepository.hpp"
#include "UsersAmountFinder.hpp"

using namespace DataAccess;

std::vector<Network::ChannelInfo> PostgreRepository::getAllChannelsList()
{
    pTable->changeTable("channels");
    auto channelListRow = pTable->Select()->columns({"*"})->execute();

    std::vector<Network::ChannelInfo> result;
    if (channelListRow.has_value())
    {
        Network::ChannelInfo channelInfo;
        for (int i = 0; i < channelListRow.value().size(); ++i)
        {
            channelInfo.channelID   = channelListRow.value()[i][0].as<std::uint64_t>();
            channelInfo.channelName = channelListRow.value()[i][1].as<std::string>();
            channelInfo.creatorID   = channelListRow.value()[i][2].as<std::uint64_t>();
            result.push_back(channelInfo);
        }
    }

    return result;
}

std::vector<Network::MessageInfo> PostgreRepository::getMessageHistoryForUser(const std::uint64_t channelID)
{
    std::vector<Network::MessageInfo> result;

    pTable->changeTable("msgs");
    auto messageHistoryRow = pTable->Select()
                                   ->columns({"*"})
                                   ->join(Utility::SQLJoinType::J_INNER, "channel_msgs", "channel_msgs.msg_id = msgs.msg_id")
                                   ->Where("channel_msgs.channel_id = " + std::to_string(channelID))
                                   ->execute();
    
    if (messageHistoryRow.has_value())
    {
        Network::MessageInfo mi;
        mi.channelID = channelID;
        for (auto i = 0; i < messageHistoryRow.value().size(); ++i)
        {
            mi.msgID    = messageHistoryRow.value()[i][0].as<std::uint64_t>();
            mi.senderID = messageHistoryRow.value()[i][1].as<std::uint64_t>();
            mi.time     = messageHistoryRow.value()[i][2].as<std::string>();
            mi.message  = messageHistoryRow.value()[i][3].as<std::string>();
            result.emplace_back(mi);
        }
    }

    return result;
}

std::vector<Network::ReplyInfo> PostgreRepository::getReplyHistoryForUser(const std::uint64_t channelID)
{
    std::vector<Network::ReplyInfo> result;

    pTable->changeTable("replies");
    auto replyHistoryRow = pTable->Select()
            ->columns({"*"})
            ->join(Utility::SQLJoinType::J_INNER, "channel_msgs", "channel_msgs.msg_id = msgs.msg_id")
            ->Where("channel_msgs.channel_id = " + std::to_string(channelID))
            ->execute();
    if(replyHistoryRow.has_value())
    {
        Network::ReplyInfo ri;

    }
    return result;
}

Utility::RegistrationCodes PostgreRepository::registerUser(const Network::RegistrationInfo& ri) const
{
    static UsersAmountFinder finder;
    // Check on existing of login and email in repository.
    if (finder.findUsersAmountWithSameEmail(ri.email) > 0)
    {
        return Utility::RegistrationCodes::EMAIL_ALREADY_EXISTS;
    }
    if (finder.findUsersAmountWithSameLogin(ri.login) > 0)
    {
        return Utility::RegistrationCodes::LOGIN_ALREADY_EXISTS;
    }

    // Data preperaion for new user inserting.
    std::tuple userData
    {
        std::pair{"email", ri.email}, 
        std::pair{"login", ri.login}, 
        std::pair{"password_hash", ri.passwordHash
    }};

    // Insert new user.
    pTable->changeTable("users");
    pTable->Insert()->columns(userData)->execute();

    return Utility::RegistrationCodes::SUCCESS;
}

Utility::StoringMessageCodes PostgreRepository::storeMessage(const Network::MessageInfo& mi)
{
    const auto firstResult = insertMessageIntoMessagesTable(mi);
    if (!firstResult.has_value())
    {
        std::cerr << "Insert message into 'msgs' table failed" << std::endl;
        return Utility::StoringMessageCodes::FAILED;
    }

    const auto currentMessageID = firstResult.value()[0][0].as<std::uint64_t>();
    
    const auto secondResult = insertIDsIntoChannelMessagesTable(mi.channelID, currentMessageID);
    if (!secondResult.has_value())
    {
        std::cerr << "Insert message into 'channel_messages' table failed" << std::endl;
        return Utility::StoringMessageCodes::FAILED;
    }

    const auto thirdResult = insertIDIntoMessageReactionsTable(currentMessageID);
    if (!thirdResult.has_value())
    {
        std::cerr << "Insert message into 'msg_reactions' table failed" << std::endl;
        return Utility::StoringMessageCodes::FAILED;
    }

    return Utility::StoringMessageCodes::SUCCESS;
}

Utility::DeletingMessageCodes PostgreRepository::deleteMessage(const Network::MessageInfo& mi) 
{
    using Utility::DeletingMessageCodes;

    pTable->changeTable("msgs");
    pTable->Delete()->Where("msg_id=" + std::to_string(mi.msgID))->Or("msg='" + mi.message + "'")->execute();
    
    // TODO: we need a better way to check state of deleting, than doing select request:
    auto messagesAmountResult = pTable->Select()->columns({"COUNT(*)"})->Where("msg_id=" + std::to_string(mi.msgID))->execute();
    if (messagesAmountResult.value()[0][0].as<std::uint64_t>() == 0)
    {
        return DeletingMessageCodes::SUCCESS;
    }

    return DeletingMessageCodes::FAILED;
}

std::optional<pqxx::result> PostgreRepository::insertMessageIntoMessagesTable(const Network::MessageInfo& mi)
{
    std::tuple dataForMsgs
    {
        std::pair{"sender_id", mi.senderID}, 
        std::pair{"send_time", mi.time.c_str()}, 
        std::pair{"msg", mi.message}
    };

    pTable->changeTable("msgs");
    return pTable->Insert()->columns(dataForMsgs)->returning({"msg_id"})->execute();
}

std::optional<pqxx::result> PostgreRepository::insertIDsIntoChannelMessagesTable(const std::uint64_t chinnelID,
                                                                                 const std::uint64_t messageID)
{
    std::tuple dataForChannelMsgs
    {
        std::pair{"channel_id", chinnelID}, 
        std::pair{"msg_id", messageID}
    };

    pTable->changeTable("channel_msgs");
    return pTable->Insert()->columns(dataForChannelMsgs)->returning({"channel_id"})->execute();
}

std::optional<pqxx::result> PostgreRepository::insertIDIntoMessageReactionsTable(const std::uint64_t messageID)
{
    pTable->changeTable("msg_reactions");
    return pTable->Insert()->columns(std::pair{"msg_id", messageID})->returning({"msg_id"})->execute();
}

std::uint64_t PostgreRepository::loginUser(const std::string& login, const std::string& pwdHash)
{
    try
    {
        pTable->changeTable("users");
        auto queryResult = pTable->Select()
                                 ->columns({"password_hash", "id"})
                                 ->Where("login='" + login + "'")
                                 ->execute().value();
        
        if (std::string(queryResult[0][0].c_str()) == pwdHash)
        {
            return queryResult[0][1].as<std::uint64_t>();
        }
        else
        {
            return 0;
        }
    }
    catch(const std::exception& e)
    {
        std::cout << e.what();
        return 0;
    }
}
