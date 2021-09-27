#include "LiteRepositories.hpp"

namespace DataAccess
{
void LiteMessageRepository::addMessages(std::vector<MessageInfo>& messages)
{
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
}  // namespace DataAccess
