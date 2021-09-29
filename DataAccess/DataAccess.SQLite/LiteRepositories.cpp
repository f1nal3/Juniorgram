#include "LiteRepositories.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

namespace DataAccess
{
void LiteMessageRepository::addMessages(std::vector<MessageInfo>& messages)
{
    if (messages.empty()) return;
    QString query = QString("INSERT INTO msgs VALUES");
    for (const auto& message : messages)
    {
        query += QString(" (%1, %2, %3, \'%4\', %5),")
                     .arg(message.msgID)
                     .arg(message.senderID)
                     .arg(message.time)
                     .arg(message.message)
                     .arg(message.channelID);
    }
    query.remove(query.size() - 1, 1);
    LiteAdapter::getInstance<LiteAdapter>()->query((query + " ON CONFLICT DO NOTHING;").toStdString());
}

void LiteMessageRepository::editMessages(std::vector<MessageInfo>& messages)
{
    if (messages.empty()) return;
    QString query = QString("INSERT INTO msgs VALUES");
    for (const auto& message : messages)
    {
        query += QString(" (%1, %2, %3, \'%4\', %5),")
                     .arg(message.msgID)
                     .arg(message.senderID)
                     .arg(message.time)
                     .arg(message.message)
                     .arg(message.channelID);
    }
    query.remove(query.size() - 1, 1);
    LiteAdapter::getInstance<LiteAdapter>()->query((query + " ON CONFLICT REPLACE;").toStdString());
}

std::vector<MessageInfo> LiteMessageRepository::getMessages(std::uint64_t channelID)
{
    auto oQuery =
        LiteAdapter::getInstance<LiteAdapter>()->query(QString("SELECT * FROM msgs WHERE channel_id = %1;").arg(channelID).toStdString());
    if (oQuery.has_value())
    {
        auto query = std::any_cast<QSqlQuery>(oQuery.value());
        if (query.size() == 1) return {};
        std::vector<MessageInfo> result;
        // msg_id BIGINT UNIQUE PRIMARY KEY NOT NULL , sender_id BIGINT NOT NULL, "
        //         "send_time BIGINT NOT NULL, msg TEXT NOT NULL, channel_id BIGINT NOT NULL"
        auto iMsg_id     = query.record().indexOf("msg_id");
        auto iChannel_id = query.record().indexOf("channel_id");
        auto iMsg        = query.record().indexOf("msg");
        auto iSend_time  = query.record().indexOf("send_time");
        auto iSender_id  = query.record().indexOf("sender_id");
        while (query.next())
        {
            auto channel_id = query.value(iChannel_id).toULongLong();
            auto sender_id  = query.value(iSender_id).toULongLong();
            auto send_time  = query.value(iSend_time).toULongLong();
            auto msg        = query.value(iMsg).toString();
            auto msg_id     = query.value(iMsg_id).toULongLong();
            result.emplace_back(MessageInfo(channel_id, sender_id, send_time, msg_id, msg));
        }
        return result;
    }
    return {};
}
std::vector<MessageInfo> LiteMessageRepository::getMessageSince(std::uint64_t channelID, std::uint64_t since)
{
    auto adapter = LiteAdapter::getInstance<LiteAdapter>();
    auto oQuery  = adapter->query(QString("SELECT * "
                                           "FROM msgs "
                                           "WHERE channel_id = %1 AND send_time > %2;")
                                      .arg(channelID)
                                      .arg(since)
                                      .toStdString());
    if (oQuery.has_value())
    {
        auto query = std::any_cast<QSqlQuery>(oQuery.value());
        if (query.size() == 1) return {};
        std::vector<MessageInfo> result;
        // msg_id BIGINT UNIQUE PRIMARY KEY NOT NULL , sender_id BIGINT NOT NULL, "
        //         "send_time BIGINT NOT NULL, msg TEXT NOT NULL, channel_id BIGINT NOT NULL"
        auto iMsg_id     = query.record().indexOf("msg_id");
        auto iChannel_id = query.record().indexOf("channel_id");
        auto iMsg        = query.record().indexOf("msg");
        auto iSend_time  = query.record().indexOf("send_time");
        auto iSender_id  = query.record().indexOf("sender_id");
        while (query.next())
        {
            auto channel_id = query.value(iChannel_id).toULongLong();
            auto sender_id  = query.value(iSender_id).toULongLong();
            auto send_time  = query.value(iSend_time).toULongLong();
            auto msg        = query.value(iMsg).toString();
            auto msg_id     = query.value(iMsg_id).toULongLong();
            result.emplace_back(MessageInfo(channel_id, sender_id, send_time, msg_id, msg));
        }
        return result;
    }
    return {};
}

std::vector<MessageInfo> LiteMessageRepository::getMessageBefore(std::uint64_t channelID, std::uint64_t before, std::uint64_t amount)
{
    auto adapter = LiteAdapter::getInstance<LiteAdapter>();
    auto oQuery  = adapter->query(QString("SELECT COUNT(%3) "
                                           "FROM msgs "
                                           "WHERE channel_id = %1 AND send_time < %2;")
                                      .arg(channelID)
                                      .arg(before)
                                      .arg(amount)
                                      .toStdString());
    if (oQuery.has_value())
    {
        auto query = std::any_cast<QSqlQuery>(oQuery.value());
        if (query.size() == 1) return {};
        std::vector<MessageInfo> result;
        // msg_id BIGINT UNIQUE PRIMARY KEY NOT NULL , sender_id BIGINT NOT NULL, "
        //         "send_time BIGINT NOT NULL, msg TEXT NOT NULL, channel_id BIGINT NOT NULL"
        auto iMsg_id     = query.record().indexOf("msg_id");
        auto iChannel_id = query.record().indexOf("channel_id");
        auto iMsg        = query.record().indexOf("msg");
        auto iSend_time  = query.record().indexOf("send_time");
        auto iSender_id  = query.record().indexOf("sender_id");
        while (query.next())
        {
            auto channel_id = query.value(iChannel_id).toULongLong();
            auto sender_id  = query.value(iSender_id).toULongLong();
            auto send_time  = query.value(iSend_time).toULongLong();
            auto msg        = query.value(iMsg).toString();
            auto msg_id     = query.value(iMsg_id).toULongLong();
            result.emplace_back(MessageInfo(channel_id, sender_id, send_time, msg_id, msg));
        }
        return result;
    }
    return {};
}

void LiteMessageRepository::removeMessages(std::vector<MessageInfo>& messages)
{
    if (messages.empty()) return;
    auto adapter = LiteAdapter::getInstance<LiteAdapter>();

    // msg_id BIGINT UNIQUE PRIMARY KEY NOT NULL , sender_id BIGINT NOT NULL, "
    //         "send_time BIGINT NOT NULL, msg TEXT NOT NULL, channel_id BIGINT NOT NULL"
    auto query = QString(
                     "REMOVE"
                     "FROM msgs "
                     "WHERE msg_id = %1")
                     .arg(messages[0].msgID);
    for (auto index = 1; index < int(messages.size()); index++)
    {
        query += QString(" OR msg_id=%1").arg(messages[index].msgID);
    }
    auto oQuery = adapter->query((query + ";").toStdString());
}
}  // namespace DataAccess
