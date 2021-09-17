#include "ServerRepositories.hpp"

namespace DataAccess
{
    std::optional<pqxx::result> toPQXX(std::optional<std::any>&& optAny)
    {
        if (optAny.has_value())
            return std::any_cast<pqxx::result>(optAny.value());
        else
            return std::nullopt;
    }

    std::vector<Network::ChannelInfo> ChannelsRepository::getAllChannelsList()
    {
        auto channelListRow = toPQXX(_adapter->query("SELECT * FROM channels"));

        std::vector<Network::ChannelInfo> result;
        if (channelListRow.has_value())
        {
            Network::ChannelInfo channelInfo;
            for (int i = 0; i < channelListRow.value().size(); ++i)
            {
                channelInfo.channelID = channelListRow.value()[i][0].as<std::uint64_t>();
                channelInfo.channelName = channelListRow.value()[i][1].as<std::string>();
                channelInfo.creatorID = channelListRow.value()[i][2].as<std::uint64_t>();
                result.push_back(channelInfo);
            }
        }

        return result;
    }

    std::uint64_t LoginRepository::loginUser(const std::string& login, std::string& pwdHash)
    {
        try
        {
            auto queryResult = toPQXX(_adapter->query("SELECT password_hash, id FROM users WHERE login=" + login)).value();

            if (std::string(queryResult[0][0].c_str()) == pwdHash)
            {
                return queryResult[0][1].as<std::uint64_t>();
            }
            else
            {
                return 0;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << e.what();
            return 0;
        }
    }

    std::vector<Network::MessageInfo> MessagesRepository::getMessageHistory(const std::uint64_t channelID)
    {
        std::vector<Network::MessageInfo> result;

        auto messageHistoryRow = toPQXX(_adapter->query(
            "SELECT * FROM msgs INNER JOIN channel_msgs ON channel_msgs.msg_id = msgs.msg_id WHERE channel_msgs.channel_id = " + std::to_string(channelID)));
        
        if (messageHistoryRow.has_value())
        {
            Network::MessageInfo mi;
            mi.channelID = channelID;
            for (auto i = 0; i < messageHistoryRow.value().size(); ++i)
            {
                mi.msgID = messageHistoryRow.value()[i][0].as<std::uint64_t>();
                mi.senderID = messageHistoryRow.value()[i][1].as<std::uint64_t>();
                mi.time = messageHistoryRow.value()[i][2].as<std::string>();
                mi.message = messageHistoryRow.value()[i][3].as<std::string>();
                result.emplace_back(mi);
            }
        }

        return result;
    }

    Utility::StoringMessageCodes MessagesRepository::storeMessage(const Network::MessageInfo& mi)
    {
        std::stringstream queryStream;
        queryStream << "INSERT FROM msgs (sender_id, send_time, msg) VALUES (";
        queryStream << mi.senderID << ", " << mi.time << ", " << mi.message << ") ";
        queryStream << "RETURNING msg_id";

        const auto firstResult = toPQXX(_adapter->query(queryStream.str()));
        if (!firstResult.has_value())
        {
            std::cerr << "Insert message into 'msgs' table failed" << std::endl;
            return Utility::StoringMessageCodes::FAILED;
        }

        const auto currentMessageID = firstResult.value()[0][0].as<std::uint64_t>();

        queryStream.clear();
        queryStream << "INSERT INTO channel_msgs (channel_id, msg_id) VALUES (";
        queryStream << mi.channelID << ", " << currentMessageID << ") ";
        queryStream << "RETURNING channel_id";
        
        const auto secondResult = toPQXX(_adapter->query(queryStream.str()));
        if (!secondResult.has_value())
        {
            std::cerr << "Insert message into 'channel_messages' table failed" << std::endl;
            return Utility::StoringMessageCodes::FAILED;
        }

        queryStream.clear();
        queryStream << "INSERT INTO msg_reactions (msg_id) VALUES (";
        queryStream << currentMessageID << ") ";
        queryStream << "RETURNING msg_id";

        const auto thirdResult = toPQXX(_adapter->query(queryStream.str()));
        if (!thirdResult.has_value())
        {
            std::cerr << "Insert message into 'msg_reactions' table failed" << std::endl;
            return Utility::StoringMessageCodes::FAILED;
        }

        return Utility::StoringMessageCodes::SUCCESS;
    }

    Utility::DeletingMessageCodes MessagesRepository::deleteMessage(const Network::MessageInfo& mi)
    {
        using Utility::DeletingMessageCodes;

        std::stringstream queryStream;
        queryStream << "DELETE FROM msgs WHERE msg_id = ";
        queryStream << std::to_string(mi.msgID) << " OR msg = '" << mi.message << "'";

        _adapter->query(queryStream.str());

        queryStream.clear();
        queryStream << "SELECT COUNT(*) FROM msgs WHERE msg_id = ";
        queryStream << std::to_string(mi.msgID);

        // TODO: we need a better way to check state of deleting, than doing select request:
        auto messagesAmountResult = toPQXX(_adapter->query(queryStream.str()));
        if (messagesAmountResult.value()[0][0].as<std::uint64_t>() == 0)
        {
            return DeletingMessageCodes::SUCCESS;
        }

        return DeletingMessageCodes::FAILED;
    }

    Utility::RegistrationCodes RegisterRepository::registerUser(const Network::RegistrationInfo& ri)
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

        std::stringstream queryStream;
        queryStream << "INSERT INTO users (email, login, password_hash) VALUES (";
        queryStream << ri.email << ", " << ri.login << ", " << ri.passwordHash << ") ";

        _adapter->query(queryStream.str());

        return Utility::RegistrationCodes::SUCCESS;
    }
    
    std::vector<Network::ReplyInfo> RepliesRepository::getReplyHistory(const std::uint64_t channelID)
    {
        std::vector<Network::ReplyInfo> result;

        std::stringstream queryStream;
        queryStream << "SELECT * FROM replies ";
        queryStream << "INNER JOIN channel_replies ";
        queryStream << "ON channel_replies.msg_id_owner = replies.msg_id_owner ";
        queryStream << "WHERE channel_replies.channel_id = " << std::to_string(channelID);

        auto replyHistoryRow = toPQXX(_adapter->query(queryStream.str()));
        if (replyHistoryRow.has_value())
        {
            Network::ReplyInfo ri;
            ri.channelID = channelID;
            for (auto i = 0; i < replyHistoryRow.value().size(); ++i)
            {
                ri.senderID = replyHistoryRow.value()[i][0].as<std::uint64_t>();
                ri.msgIdOwner = replyHistoryRow.value()[i][1].as<std::uint64_t>();
                ri.msgID = replyHistoryRow.value()[i][2].as<std::uint64_t>();
                ri.message = replyHistoryRow.value()[i][3].as<std::string>();
                result.emplace_back(ri);
            }

        }
        return result;
    }
}
