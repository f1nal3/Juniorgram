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
            // at(0) - msg_id
            mi.userID = messageHistoryRow.value()[i][1].as<std::uint64_t>();
            mi.time = messageHistoryRow.value()[i][2].as<std::string>();
            mi.message = messageHistoryRow.value()[i][3].as<std::string>();
            result.emplace_back(mi);
        }
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

Utility::StoringMessageCodes PostgreRepository::storeMessage(const Network::MessageInfo& msi)
{
    const auto firstResult = insertMessageIntoMessagesTable(msi);
    if (!firstResult.has_value())
    {
        std::cerr << "Insert message into 'msgs' table failed" << std::endl;
        return Utility::StoringMessageCodes::FAILED;
    }

    const auto currentMessageID = firstResult.value()[0][0].as<std::uint64_t>();
    
    const auto secondResult = insertIDsIntoChannelMessagesTable(msi.channelID, currentMessageID);
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

std::optional<pqxx::result> PostgreRepository::insertMessageIntoMessagesTable(const Network::MessageInfo& msi)
{
    std::tuple dataForMsgs
    {
        std::pair{"sender_id", msi.userID}, 
        std::pair{"send_time", msi.time.c_str()}, 
        std::pair{"msg", msi.message}
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
