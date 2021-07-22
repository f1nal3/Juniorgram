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

void PostgreRepository::storeMessage(const Network::MessageInfo& message, const std::uint64_t channelID)
{
    std::string timeStampStr = Utility::nowTimeStampStr();

    std::tuple dataForMsgs
    {
        std::pair{"sender_id", message.userID},
        std::pair{"send_time", timeStampStr.c_str()},
        std::pair{"msg", message.message }
    };
    pTable->changeTable("msgs");
    pTable->Insert()->columns(dataForMsgs)->execute();
    
    // ID will not be autoincremented in the future. Later we are going to use postgre
    // alghorithms to create it.
    std::uint64_t msgID = pTable->Select()
                                ->columns({"max(msg_id)"})
                                ->execute()
                                .value()[0][0].as<std::uint64_t>() + 1;
    std::tuple dataForChannelMsgs
    {
        std::pair{"channel_id", channelID},
        std::pair{"msg_id", msgID}
    };
    
    pTable->changeTable("channel_msgs");
    pTable->Insert()->columns(dataForChannelMsgs)->execute();
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