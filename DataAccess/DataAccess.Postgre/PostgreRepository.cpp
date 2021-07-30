#include "PostgreRepository.hpp"
#include "UsersAmountFinder.hpp"

using namespace DataAccess;

std::vector<std::string> PostgreRepository::getAllChannelsList()
{
    std::vector<std::string> result;

    pTable->changeTable("channels");
    auto channelListRow = pTable->Select()->columns({"channel_name"})->execute();

    if (channelListRow.has_value())
    {
        for (auto item : channelListRow.value())
        {
            std::cout << item.at(0).c_str() << '\n';
            result.emplace_back(std::string(item.at(0).c_str()));
        }
    }

    return result;
}

std::vector<std::string> PostgreRepository::getMessageHistoryForUser(const std::uint64_t channelID)
{
    std::vector<std::string> result;

    pTable->changeTable("msgs");
    auto messageHistoryRow = pTable->Select()
                                   ->columns({"msg"})
                                   ->join(Utility::SQLJoinType::J_INNER, "channel_msgs", "channel_msgs.msg_id = msgs.msg_id")
                                   ->Where("channel_msgs.channel_id = " + std::to_string(channelID))
                                   ->execute();
    
    if (messageHistoryRow.has_value())
    {
        for (auto message : messageHistoryRow.value())
        {
            std::cout << message.at(0).c_str() << '\n';
            result.emplace_back(std::string(message.at(0).c_str()));
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

Utility::StoreMessageCodes PostgreRepository::storeMessage(const Network::MessageStoringInfo& msi)
{
    const auto firstResult = insertMessageIntoMessagesTable(msi);
    if (!firstResult.has_value())
    {
        std::cerr << "Insert message into 'msgs' table failed" << std::endl;
        return Utility::StoreMessageCodes::FAILED;
    }

    const auto currentMessageID = firstResult.value()[0][0].as<std::uint64_t>();
    
    const auto secondResult = insertIDsIntoChannelMessagesTable(msi.channelID, currentMessageID);
    if (!secondResult.has_value())
    {
        std::cerr << "Insert message into 'channel_messages' table failed" << std::endl;
        return Utility::StoreMessageCodes::FAILED;
    }

    const auto thirdResult = insertIDIntoMessageReactionsTable(currentMessageID);
    if (!thirdResult.has_value())
    {
        std::cerr << "Insert message into 'msg_reactions' table failed" << std::endl;
        return Utility::StoreMessageCodes::FAILED;
    }

    return Utility::StoreMessageCodes::SUCCESS;
}

std::optional<pqxx::result> PostgreRepository::insertMessageIntoMessagesTable(const Network::MessageStoringInfo& msi)
{
    std::string timeStampStr = Utility::nowTimeStampStr();
    std::tuple dataForMsgs
    {
        std::pair{"sender_id", msi.userID}, 
        std::pair{"send_time", timeStampStr.c_str()}, 
        std::pair{"msg", msi.message}
    };

    pTable->changeTable("msgs");
    return pTable->Insert()->columns(dataForMsgs)->returning({"id"})->execute();
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
    return pTable->Insert()->columns(dataForChannelMsgs)->execute();
}

std::optional<pqxx::result> PostgreRepository::insertIDIntoMessageReactionsTable(const std::uint64_t messageID)
{
    pTable->changeTable("msg_reactions");
    return pTable->Insert()->columns(std::pair{"msg_id", messageID})->execute();
}
