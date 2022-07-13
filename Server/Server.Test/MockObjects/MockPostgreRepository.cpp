#include "MockPostgreRepository.hpp"

namespace MockRepository
{
std::vector<Network::ChannelInfo> testChannelsRepository::getAllChannelsList() const
{
    _pTable->changeTable("channels");
    auto                 channelListRow = _pTable->Select()->columns({"*"})->execute();
    Network::ChannelInfo channelInfo;

    channelInfo.channelID   = 15;
    channelInfo.channelName = "testServer";
    channelInfo.creatorID   = 1;

    auto adapter = _pTable->getAdapter();
    auto insertResult =
        adapter->query("INSERT INTO channels(channelID, channelName, creatorID) VALUES (" + std::to_string(channelInfo.channelID) + "," +
                       channelInfo.channelName + ", " + std::to_string(channelInfo.creatorID));

    std::vector<Network::ChannelInfo> result;
    if (channelListRow.has_value())
    {
        for (auto&& value : channelListRow.value())
        {
            channelInfo.channelID   = value[0].as<std::uint64_t>();
            channelInfo.channelName = value[1].as<std::string>();
            channelInfo.creatorID   = value[2].as<std::uint64_t>();
            result.push_back(channelInfo);
        }
    }

    Base::Logger::FileLogger::getInstance().log
    (
        "All channels are successfully given to client\n",
        Base::Logger::LogLevel::INFO
    );

    return result;
}

std::vector<uint64_t> testChannelsRepository::getChannelSubscriptionList(uint64_t userID) const
{
    _pTable->changeTable("user_channels");

    auto listSubscriptionChannel = _pTable->Select()->columns({"channel_id"})->Where("user_id = " + std::to_string(userID))->execute();
    std::vector<uint64_t> result;
    if (listSubscriptionChannel.has_value())
    {
        for (auto&& value : listSubscriptionChannel.value())
        {
            auto channelID = value[0].as<uint64_t>();
            result.push_back(channelID);
        }
    }

    Base::Logger::FileLogger::getInstance().log
    (
        "List of subscriptions of channel id: " 
        + std::to_string(listSubscriptionChannel.value()[0][0].as<uint32_t>()) +
            " was successfully given\n",
        Base::Logger::LogLevel::INFO
    );

    return result;
}

std::vector<Network::MessageInfo> testMessagesRepository::getMessageHistoryForUser(std::uint64_t channelID) const
{
    std::vector<Network::MessageInfo> result;

    _pTable->changeTable("msgs");
    auto messageHistoryRow = _pTable->Select()
                                 ->columns({"msgs.msg_id, msgs.sender_id, extract(epoch from msgs.send_time) * 1000, msgs.msg, "
                                            "users.login, users.id, "
                                            "coalesce(array_length(msg_reactions.likes, 1), 0), "
                                            "coalesce(array_length(msg_reactions.dislikes, 1), 0), "
                                            "coalesce(array_length(msg_reactions.fires, 1), 0), "
                                            "coalesce(array_length(msg_reactions.cats, 1), 0), "
                                            "coalesce(array_length(msg_reactions.smiles, 1), 0)"})
                                 ->join(Utility::SQLJoinType::J_INNER, "channel_msgs", "channel_msgs.msg_id = msgs.msg_id")
                                 ->join(Utility::SQLJoinType::J_INNER, "users", "users.id = msgs.sender_id")
                                 ->join(Utility::SQLJoinType::J_INNER, "msg_reactions", "msg_reactions.msg_id = msgs.msg_id")
                                 ->Where("channel_msgs.channel_id = " + std::to_string(channelID))
                                 ->execute();

    Network::MessageInfo mi;
    mi.msgID        = 1;
    mi.senderID     = 1;
    mi.message      = "Hello, Juniorgram!!";
    mi.userLogin    = "memorisecodead";
    mi.reactions[4] = {1};

    auto adapter      = _pTable->getAdapter();
    auto insertResult = adapter->query("INSERT INTO msgs(msgID, senderID, time, message, userLogin, reactions) VALUES (" +
                                       std::to_string(mi.msgID) + "," + std::to_string(mi.senderID) + ", " + std::to_string(mi.time) +
                                       ", " + mi.message + ", " + mi.userLogin + ", " + std::to_string(mi.reactions[4]));

    if (messageHistoryRow.has_value())
    {
        mi.channelID = channelID;

        for (auto&& value : messageHistoryRow.value())
        {
            mi.msgID        = value[0].as<std::uint64_t>();
            mi.senderID     = value[1].as<std::uint64_t>();
            mi.time         = static_cast<std::uint64_t>(value[2].as<double>());
            mi.message      = value[3].as<std::string>();
            mi.userLogin    = value[4].as<std::string>();
            mi.reactions[0] = value[6].as<std::uint32_t>();
            mi.reactions[1] = value[7].as<std::uint32_t>();
            mi.reactions[2] = value[8].as<std::uint32_t>();
            mi.reactions[3] = value[9].as<std::uint32_t>();
            mi.reactions[4] = value[10].as<std::uint32_t>();
            result.emplace_back(mi);
        }
    }

    Base::Logger::FileLogger::getInstance().log
    (
        '[' + "channel id: " + std::to_string(channelID) 
        + "] Message history was successfully given to client\n",
        Base::Logger::LogLevel::INFO
    );

    return result;
}

std::vector<Network::ReplyInfo> testRepliesRepository::getReplyHistoryForUser(std::uint64_t channelID) const
{
    std::vector<Network::ReplyInfo> result;

    _pTable->changeTable("replies");
    auto replyHistoryRow =
        _pTable->Select()
            ->columns({"replies.sender_id, replies.msg_id_owner, replies.msg_id_ref, replies.msg, users.login, users.id"})
            ->join(Utility::SQLJoinType::J_INNER, "channel_replies", "channel_replies.msg_id_owner = replies.msg_id_owner")
            ->join(Utility::SQLJoinType::J_INNER, "users", "users.id = replies.sender_id")
            ->Where("channel_replies.channel_id = " + std::to_string(channelID))
            ->execute();
    if (replyHistoryRow.has_value())
    {
        Network::ReplyInfo ri;
        ri.channelID = channelID;
        for (auto i = 0; i < replyHistoryRow.value().size(); ++i)
        {
            ri.senderID   = replyHistoryRow.value()[i][0].as<std::uint64_t>();
            ri.msgIdOwner = replyHistoryRow.value()[i][1].as<std::uint64_t>();
            ri.msgID      = replyHistoryRow.value()[i][2].as<std::uint64_t>();
            ri.message    = replyHistoryRow.value()[i][3].as<std::string>();
            ri.userLogin  = replyHistoryRow.value()[i][4].as<std::string>();
            result.emplace_back(ri);
        }
    }

    Base::Logger::FileLogger::getInstance().log
    (
        "Reply history was successfully given\n",
        Base::Logger::LogLevel::INFO
    );

    return result;
}

Utility::StoringMessageCodes testMessagesRepository::storeMessage(Network::MessageInfo& msi) const
{
    const auto firstResult = insertMessageIntoMessagesTable(msi);
    if (!firstResult.has_value())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            '[' + "channel id:" + std::to_string(msi.channelID) 
            + "] Inserting message into 'msgs' table failed\n",
            Base::Logger::LogLevel::ERR
        );

        return Utility::StoringMessageCodes::FAILED;
    }

    const auto currentMessageID = firstResult.value()[0][0].as<std::uint64_t>();

    const auto secondResult = insertIDsIntoChannelMessagesTable(msi.channelID, currentMessageID);
    if (!secondResult.has_value())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            '[' + "channel id: " + std::to_string(msi.channelID)
            + "] Inserting message into 'channel_messages' table failed\n",
            Base::Logger::LogLevel::ERR
        );

        return Utility::StoringMessageCodes::FAILED;
    }

    const auto thirdResult = insertIDIntoMessageReactionsTable(currentMessageID);
    if (!thirdResult.has_value())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            '[' + "channel id: " + std::to_string(msi.channelID) 
            + "] Inserting message into 'msg_reactions' table failed\n",
            Base::Logger::LogLevel::ERR
        );

        return Utility::StoringMessageCodes::FAILED;
    }
    Base::Logger::FileLogger::getInstance().log
    (
        '[' + "channel id: " + std::to_string(msi.channelID)
        + "] Message have stored successfully\n",
        Base::Logger::LogLevel::INFO
    );

    return Utility::StoringMessageCodes::SUCCESS;
}

Utility::StoringReplyCodes testRepliesRepository::storeReply(const Network::ReplyInfo& rsi) const
{
    const auto firstResult = insertReplyIntoRepliesTable(rsi);
    if (!firstResult.has_value())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "Inserting reply into 'replies' table failed\n", 
            Base::Logger::LogLevel::ERR
        );

        return Utility::StoringReplyCodes::FAILED;
    }

    const auto currentReplyID = firstResult.value()[0][0].as<std::uint64_t>();

    const auto secondResult = insertIDsIntoChannelRepliesTable(rsi.channelID, currentReplyID);
    if (!secondResult.has_value())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "Inserting reply into 'channel_replies' table failed\n", 
            Base::Logger::LogLevel::ERR
        );

        return Utility::StoringReplyCodes::FAILED;
    }
    Base::Logger::FileLogger::getInstance().log
    (
        "[channel id: " + std::to_string(rsi.channelID) 
        + "] Reply successfully stored in database\n", 
        Base::Logger::LogLevel::INFO
    );

    return Utility::StoringReplyCodes::SUCCESS;
}

Utility::DeletingMessageCodes testMessagesRepository::deleteMessage(Network::MessageInfo& mi) const
{
    mi.message = "Hello, Juniorgram!!";
    mi.msgID   = 7;

    _pTable->changeTable("msgs");
    _pTable->Delete()->Where("msg_id=" + std::to_string(mi.msgID))->Or("msg='" + mi.message + "'")->execute();

    auto messagesAmountResult = _pTable->Select()->columns({"COUNT(*)"})->Where("msg_id=" + std::to_string(mi.msgID))->execute();

    if (messagesAmountResult.value()[0][0].as<std::uint64_t>() == 0)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "Message have deleted successfully\n", 
            Base::Logger::LogLevel::INFO
        );

        return Utility::DeletingMessageCodes::SUCCESS;
    }
    Base::Logger::FileLogger::getInstance().log
    (
        "Message deleting failed\n",
        Base::Logger::LogLevel::ERR
    );

    return Utility::DeletingMessageCodes::FAILED;
}

Utility::EditingMessageCodes testMessagesRepository::editMessage(Network::MessageInfo& mi) const
{
    _pTable->changeTable("msgs");

    auto isPresentInTable = _pTable->Select()
                                ->columns({"*"})
                                ->Where("msg_id =" + std::to_string(mi.msgID))
                                ->And("msgs.sender_id = " + std::to_string(mi.senderID))
                                ->execute();

    mi.userLogin    = "memorisecodead";
    mi.message      = "Hello, Juniorgram!!";
    mi.channelID    = 15;
    mi.msgID        = 7;
    mi.reactions[4] = {1};
    mi.senderID     = 1;

    auto adapter      = _pTable->getAdapter();
    auto insertResult = adapter->query("INSERT INTO msgs(message, msgID, senderID) VALUES (" + std::to_string(mi.channelID) + "," +
                                       mi.message + "," + std::to_string(mi.msgID) + ", " + std::to_string(mi.senderID));

    if (!isPresentInTable.has_value())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            '[' + "channel id: " + std::to_string(mi.channelID) + ']'
            + " Editing message failed because message was not found\n",
            Base::Logger::LogLevel::ERR
        );

        return Utility::EditingMessageCodes::FAILED;
    }

    _pTable->Update()->fields(std::pair{"msg", mi.message})->Where("msg_id =" + std::to_string(mi.msgID))->execute();

    Base::Logger::FileLogger::getInstance().log
    (
        '[' + "channel id: " + std::to_string(mi.channelID) 
        + ']' + " Editing message was successful\n",
        Base::Logger::LogLevel::INFO
    );

    return Utility::EditingMessageCodes::SUCCESS;
}

Utility::ReactionMessageCodes testMessagesRepository::updateMessageReactions(Network::MessageInfo& mi) const
{
    using Utility::ReactionMessageCodes;

    const std::vector<std::string> reactionNames = {"likes", "dislikes", "fires", "cats", "smiles"};

    auto reactionInfo = std::find_if(mi.reactions.cbegin(), mi.reactions.cend(), [](std::pair<std::uint32_t, std::uint32_t> p) {
        return p.second == std::numeric_limits<std::uint32_t>::max();
    });

    const uint64_t msgID        = 1;
    auto           adapter      = _pTable->getAdapter();
    auto           insertResult = adapter->query("INSERT INTO msg_reactions(msg_id) VALUES (" + msgID);

    if (reactionInfo == mi.reactions.end())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "Updating message reaction failed because providing reaction was not found\n",
                                                    Base::Logger::LogLevel::ERR
        );

        return ReactionMessageCodes::FAILED;
    }

    std::uint32_t reactionID   = reactionInfo->first;
    std::string   reactionName = "NULL";

    if (reactionID < reactionNames.size())
    {
        reactionName = reactionNames[reactionID];
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "Reaction with id: " + std::to_string(reactionID)
            + "is not supported by server\n",
            Base::Logger::LogLevel::ERR);

        return ReactionMessageCodes::FAILED;
    }

    _pTable->changeTable("msg_reactions");
    std::optional<pqxx::result> userQueryResult = _pTable->Select()
                                                      ->columns({"*"})
                                                      ->Where("msg_id=" + std::to_string(mi.msgID))
                                                      ->And(std::to_string(mi.senderID) + " = ANY(" + reactionName + ");")
                                                      ->execute();

    if (userQueryResult.has_value())
    {
        adapter->query("UPDATE msg_reactions SET " + reactionName + " = array_remove(" + reactionName + ", " + std::to_string(mi.senderID) +
                       ") WHERE msg_id = " + std::to_string(mi.msgID) + ";");
    }
    else
    {
        adapter->query("UPDATE msg_reactions SET " + reactionName + " = array_append(" + reactionName + ", " + std::to_string(mi.senderID) +
                       ") WHERE msg_id = " + std::to_string(mi.msgID) + ";");
    }

    Base::Logger::FileLogger::getInstance().log
    (
        "Message reactions updated\n", 
        Base::Logger::LogLevel::INFO
    );

    return ReactionMessageCodes::SUCCESS;
}

Utility::RegistrationCodes testRegisterRepository::registerUser(Network::RegistrationInfo& ri) const
{
    const std::string testEmail    = "demonstakingoverme@epam.co";
    const std::string testLogin    = "memorisecodead";
    const std::string testPassword = "12juniorgramMargroinuj";
    const std::string testPWDHash  = Base::Hashing::SHA_256(testPassword, testLogin);

    ri.email        = testEmail;
    ri.login        = testLogin;
    ri.passwordHash = testPWDHash;

    static UsersAmountFinder finder;

    if (finder.findUsersAmountWithSameEmail(ri.email) > 0)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "This email (" + ri.email + ") is already used\n", 
            Base::Logger::LogLevel::INFO
        );

        return Utility::RegistrationCodes::EMAIL_ALREADY_EXISTS;
    }

    if (finder.findUsersAmountWithSameLogin(ri.login) > 0)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "This login (" + ri.login + ") is already used\n", 
            Base::Logger::LogLevel::INFO
        );

        return Utility::RegistrationCodes::LOGIN_ALREADY_EXISTS;
    }

    auto adapter = _pTable->getAdapter();
    auto insertResult =
        adapter->query("INSERT INTO users(login, passwordHash, email) VALUES (" + ri.login + "," + ri.passwordHash + "," + ri.email);

    std::tuple userData{std::pair{"email", ri.email}, std::pair{"login", ri.login}, std::pair{"password_hash", ri.passwordHash}};

    _pTable->changeTable("users");

    _pTable->Insert()->columns(userData)->execute();

    Base::Logger::FileLogger::getInstance().log
    (
        "User successfully registered\n",
        Base::Logger::LogLevel::INFO
    );

    return Utility::RegistrationCodes::SUCCESS;
}

std::uint64_t testLoginRepository::loginUser([[maybe_unused]] std::string& login, [[maybe_unused]] std::string& pwdHash) const
{
    try
    {
        const std::string testLogin    = "memorisecodead";
        const std::string testPassword = "12juniorgramMargroinuj";
        const std::string testPWDHash  = Base::Hashing::SHA_256(testPassword, testLogin);

        Network::LoginInfo loginInfo;
        loginInfo.login   = testLogin;
        loginInfo.pwdHash = testPassword;

        _pTable->changeTable("users");
        auto queryResult = _pTable->Select()->columns({"password_hash", "id"})->Where("login='" + loginInfo.login + "'")->execute().value();

        auto adapter      = _pTable->getAdapter();
        auto insertResult = adapter->query("INSERT INTO users(login, pwdHash) VALUES (" + loginInfo.login + "," + loginInfo.pwdHash);

        if (std::string(queryResult[0][0].c_str()) == loginInfo.pwdHash)
        {
            Base::Logger::FileLogger::getInstance().log(
                std::to_string(queryResult[0][1].as<std::uint64_t>()) + " user logged successfully\n", Base::Logger::LogLevel::INFO);

            return queryResult[0][1].as<std::uint64_t>();
        }
        else
        {
            return 0;
        }
    }
    catch (const std::exception& e)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            std::string(e.what() + '\n'), 
            Base::Logger::LogLevel::ERR
        );

        return 0;
    }
}

Utility::ChannelDeleteCode testChannelsRepository::deleteChannel(Network::ChannelDeleteInfo& channel) const
{
    _pTable->changeTable("channels");
    auto findChannel = _pTable->Select()->columns({"creator_id, id"})->Where("channel_name = '" + channel.channelName + "'")->execute();

    channel.channelName = "testServer";

    if (!findChannel.has_value())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "Deleting channel failed because channel was not found\n ",
            Base::Logger::LogLevel::WARNING
        );

        return Utility::ChannelDeleteCode::CHANNEL_NOT_FOUND;
    }

    auto creatorlID = findChannel.value()[0][0].as<uint64_t>();
    auto channelID  = findChannel.value()[0][1].as<uint64_t>();
    if (creatorlID != channel.creatorID)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            '[' + "channel id: " + std::to_string(channelID) + "] User id " +
            std::to_string(creatorlID) + " is not the creator of this channel\n",
            Base::Logger::LogLevel::INFO
        );

        return Utility::ChannelDeleteCode::CHANNEL_IS_NOT_USER;
    }

    _pTable->changeTable("channel_msgs");
    auto msgs = _pTable->Select()->columns({"msg_id"})->Where("channel_id = " + std::to_string(channelID))->execute();

    _pTable->Delete()->Where("channel_id = " + std::to_string(channelID))->execute();
    if (msgs.has_value())
    {
        _pTable->changeTable("msgs");
        for (auto&& msg : msgs.value())
        {
            _pTable->Delete()->Where("msg_id = " + std::to_string(msg[0].as<std::uint64_t>()))->execute();
        }
    }

    _pTable->changeTable("channels");
    auto result = _pTable->Delete()
                      ->Where("channel_name = '" + channel.channelName + "'")
                      ->And("creator_id = " + std::to_string(channel.creatorID))
                      ->execute();
    if (result.has_value())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            '[' + "channel id: " + std::to_string(channelID) 
            + "] Deleting channel was failed\n",
            Base::Logger::LogLevel::ERR
        );

        return Utility::ChannelDeleteCode::FAILED;
    }

    Base::Logger::FileLogger::getInstance().log
    (
        '[' + "channel id: " + std::to_string(channelID) 
        + "] Deleting channel was successful\n",
        Base::Logger::LogLevel::INFO
    );

    return Utility::ChannelDeleteCode::SUCCESS;
}

Utility::ChannelCreateCodes testChannelsRepository::createChannel(Network::ChannelInfo& channel) const
{
    _pTable->changeTable("channels");
    auto findChannel = _pTable->Select()->columns({"channel_name"})->Where("channel_name = '" + channel.channelName + "'")->execute();

    channel.channelName = "testServer";

    auto adapter      = _pTable->getAdapter();
    auto insertResult = adapter->query("INSERT INTO channels(channelName) VALUES (" + channel.channelName);

    if (findChannel.has_value() && findChannel.value()[0][0].as<std::string>() == channel.channelName)
    {
            Base::Logger::FileLogger::getInstance().log
            (
                "Creating channel '" + channel.channelName 
                + "' failed since this channel exists\n",
                Base::Logger::LogLevel::INFO
            );

            return Utility::ChannelCreateCodes::CHANNEL_ALREADY_CREATED;
    }

    std::tuple channelData{std::pair{"channel_name", channel.channelName}, std::pair{"creator_id", channel.creatorID},
                           std::pair{"user_limit", 1'000'000}};

    auto result = _pTable->Insert()->columns(channelData)->execute();

    if (result.has_value())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "Creating channel '" + channel.channelName 
            + "' failed due to error with adding in channels table\n",
            Base::Logger::LogLevel::ERR
        );

        return Utility::ChannelCreateCodes::FAILED;
    }

    auto newChannel   = _pTable->Select()->columns({"id"})->Where("channel_name = '" + channel.channelName + "'")->execute();
    auto IDNewChannel = newChannel.value()[0][0].as<uint64_t>();

    std::tuple SubscribNewChannelData{std::pair{"user_id", channel.creatorID}, std::pair{"channel_id", IDNewChannel}};

    _pTable->changeTable("user_channels");
    _pTable->Insert()->columns(SubscribNewChannelData)->execute();

    Base::Logger::FileLogger::getInstance().log
    (
        '[' + "channel id: " + std::to_string(IDNewChannel) 
        + "] Creating channel was successful\n",
        Base::Logger::LogLevel::INFO
    );

    return Utility::ChannelCreateCodes::SUCCESS;
}

Utility::ChannelLeaveCodes testChannelsRepository::leaveChannel(Network::ChannelLeaveInfo& channel) const
{
    _pTable->changeTable("channels");
    auto findIdChannel = _pTable->Select()->columns({"id"})->Where("channel_name = '" + channel.channelName + "'")->execute();

    _pTable->changeTable("user_channels");

    if (!findIdChannel.has_value())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "Leaving from channel failed because channel was not found\n ",
             Base::Logger::LogLevel::ERR
        );

        return Utility::ChannelLeaveCodes::CHANNEL_NOT_FOUND;
    }
    auto findChannel = _pTable->Select()
                           ->columns({"*"})
                           ->Where("user_id = " + std::to_string(channel.creatorID) + " AND " +
                                   "channel_id = " + std::to_string(findIdChannel.value()[0][0].as<std::uint64_t>()))
                           ->execute();
    if (findChannel.has_value())
    {
        auto result = _pTable->Delete()
                          ->Where("user_id = " + std::to_string(findChannel.value()[0][0].as<std::uint64_t>()) + " AND " +
                                  "channel_id = " + std::to_string(findChannel.value()[0][1].as<std::uint64_t>()))
                          ->execute();
        if (result.has_value())
        {
            Base::Logger::FileLogger::getInstance().log
            (
                '[' + "channel id: " + std::to_string(findChannel.value()[0][1].as<std::uint64_t>()) +
                    "] Leaving from channel by user id " + std::to_string(findChannel.value()[0][0].as<std::uint64_t>()) + " failed\n",
                Base::Logger::LogLevel::ERR
            );

            return Utility::ChannelLeaveCodes::FAILED;
        }
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "Leaving from channel failed because channel was not found\n ",
            Base::Logger::LogLevel::ERR
        );

        return Utility::ChannelLeaveCodes::CHANNEL_NOT_FOUND;
    }

    Base::Logger::FileLogger::getInstance().log
    (
        '[' + "channel id: " + std::to_string(findIdChannel.value()[0][0].as<std::uint64_t>()) 
        + "] Leaving from channel is successful\n",
        Base::Logger::LogLevel::INFO
    );

    return Utility::ChannelLeaveCodes::SUCCESS;
}

Utility::ChannelSubscribingCodes testChannelsRepository::subscribeToChannel(Network::ChannelSubscriptionInfo& channel) const
{
    _pTable->changeTable("user_channels");
    auto channel_id              = std::to_string(channel.channelID);
    auto listSubscriptionChannel = _pTable->Select()
                                       ->columns({"*"})
                                       ->Where("channel_id = " + channel_id)
                                       ->And("user_id = " + std::to_string(channel.userID))
                                       ->And("(SELECT COUNT(*) from user_channels WHERE channel_id=" + channel_id +
                                             ") < (SELECT user_limit FROM channels WHERE id=" + channel_id + ")")
                                       ->execute();

    if (listSubscriptionChannel.has_value() && (listSubscriptionChannel.value()[0][0].as<std::uint64_t>() == channel.userID) &&
            (listSubscriptionChannel.value()[0][1].as<std::uint64_t>() == channel.channelID))
    {
            Base::Logger::FileLogger::getInstance().log
            (
                "[channel id: " + std::to_string(channel.channelID)
                + "] User already subscribed to the channel\n",
                Base::Logger::LogLevel::INFO
            );

            return Utility::ChannelSubscribingCodes::CHANNEL_HAS_ALREADY_BEEN_SIGNED;
    }

    std::tuple userData{std::pair{"user_id", channel.userID}, std::pair{"channel_id", channel.channelID}};

    auto result = _pTable->Insert()->columns(userData)->execute();
    if (result.has_value())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[channel id: " + std::to_string(channel.channelID)
            + "] Subscribing failed due to error with inserting data\n",
            Base::Logger::LogLevel::ERR
        );

        return Utility::ChannelSubscribingCodes::FAILED;
    }

    Base::Logger::FileLogger::getInstance().log
    (
        "[channel id: " + std::to_string(channel.channelID) 
        + "] User successfully subscribed to the channel\n",
        Base::Logger::LogLevel::INFO
    );

    return Utility::ChannelSubscribingCodes::SUCCESS;
}

std::optional<pqxx::result> testRepliesRepository::insertIDsIntoChannelRepliesTable(const std::uint64_t channelID,
                                                                                    const std::uint64_t replyID) const
{
    std::tuple dataForChannelReplies{std::pair{"channel_id", channelID}, std::pair{"msg_id_owner", replyID}};

    _pTable->changeTable("channel_replies");
    return _pTable->Insert()->columns(dataForChannelReplies)->returning({"channel_id"})->execute();
}

std::optional<pqxx::result> testRepliesRepository::insertReplyIntoRepliesTable(const Network::ReplyInfo& rsi) const
{
    _pTable->changeTable("msgs");
    auto lastMsgID = _pTable->Select()->columns({"MAX(msg_id)"})->execute();

    std::tuple dataForReplies{std::pair{"sender_id", rsi.senderID}, std::pair{"msg_id_owner", lastMsgID.value()[0][0].as<std::uint64_t>()},
                              std::pair{"msg_id_ref", rsi.msgID}, std::pair{"msg", rsi.message}};

    _pTable->changeTable("replies");
    return _pTable->Insert()->columns(dataForReplies)->returning({"msg_id_owner"})->execute();
}

std::optional<pqxx::result> testMessagesRepository::insertMessageIntoMessagesTable(const Network::MessageInfo& msi) const
{
    auto adapter = _pTable->getAdapter();

    auto result = adapter->query("INSERT INTO msgs(sender_id, send_time, msg) VALUES (" + std::to_string(msi.senderID) + ", to_timestamp(" +
                                 std::to_string(msi.time) + " / 1000.0) AT TIME ZONE 'utc', " +
                                 Utility::CheckForSQLSingleQuotesProblem(msi.message) + ") RETURNING msg_id;");

    return {std::any_cast<pqxx::result>(result.value())};
}

std::optional<pqxx::result> testMessagesRepository::insertIDsIntoChannelMessagesTable(const std::uint64_t channelID,
                                                                                      const std::uint64_t messageID) const
{
    std::tuple dataForChannelMsgs{std::pair{"channel_id", channelID}, std::pair{"msg_id", messageID}};
    _pTable->changeTable("channel_msgs");
    return _pTable->Insert()->columns(dataForChannelMsgs)->returning({"channel_id"})->execute();
}

std::optional<pqxx::result> testMessagesRepository::insertIDIntoMessageReactionsTable(const std::uint64_t messageID) const
{
    _pTable->changeTable("msg_reactions");
    return _pTable->Insert()->columns(std::pair{"msg_id", messageID})->returning({"msg_id"})->execute();
}

Utility::DirectMessageStatus testDirectMessageRepository::addDirectChat(uint64_t user_id, uint64_t receiverId) const
{
    if (user_id == 0)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "User doesn't exist\n", 
            Base::Logger::LogLevel::ERR
        );

        return Utility::DirectMessageStatus::FAILED;
    }

    _pTable->changeTable("channels");
    auto adapter   = _pTable->getAdapter();
    auto minUserId = std::to_string(std::min(user_id, receiverId));
    auto maxUserId = std::to_string(std::max(user_id, receiverId));
    auto result    = adapter->query(
        "create extension if not exists pgcrypto;\n"
        "INSERT INTO channels VALUES (DEFAULT, encode(digest(concat(" +
        minUserId + ", " + maxUserId + "),'sha384'),'base64')," + minUserId + ",2) ON CONFLICT DO NOTHING;");

    result = adapter->query("SELECT id FROM channels WHERE channel_name=encode(digest(concat(" + minUserId + ", " + maxUserId +
                            "),'sha384'),'base64');");

    if (result.has_value())
    {
        auto cId       = std::any_cast<pqxx::result>(result.value());
        auto channelId = std::to_string(cId[0][0].as<uint64_t>());
        adapter->query("INSERT INTO user_channels VALUES (" + minUserId + "," + channelId + "), (" + maxUserId + "," + channelId +
                       ")ON CONFLICT DO NOTHING;");

        Base::Logger::FileLogger::getInstance().log
        (
            "Direct chat successfully created\n", 
            Base::Logger::LogLevel::INFO
        );

        return Utility::DirectMessageStatus::SUCCESS;
    }

    Base::Logger::FileLogger::getInstance().log
    (
        "Creating direct chat failed\n",
        Base::Logger::LogLevel::ERR
    );

    return Utility::DirectMessageStatus::FAILED;
}
}  // namespace MockRepository
