#pragma once
#include "DataAccess/IClientRepositories.hpp"

#include "LiteAdapter.hpp"

namespace DataAccess
{
class AbstractLiteRepository
{
public:
    AbstractLiteRepository(std::shared_ptr<IAdapter>& adapter);
    bool ensureTable(const QString& table_name, const QString& fields);

private:
    std::shared_ptr<IAdapter>& _adapter;
};

struct MessageInfo
{
    /// channel identifier
    std::uint64_t channelID{};
    /// text of a message
    QString message{};
    /// message unique identifier
    std::uint64_t msgID{};
    /// message senders identifier
    std::uint64_t senderID{};
    /// message recipient identifier (unused while we do not have direct messages)
    std::uint64_t recipientID{};
    ///
    std::uint64_t time = 0;

    /// Default MessageIndo constructor
    MessageInfo() = delete;
    /// MessageIndo constructor with initializing list
    explicit MessageInfo(const std::uint64_t channelID, const std::uint64_t senderID, const std::uint64_t time, const std::uint64_t msgID,
                         QString text)
        : channelID(channelID), message(std::move(text)), msgID(msgID), senderID(senderID), time(time)
    {
    }

    /// Compare messages by time
    friend bool operator<(const MessageInfo& first, const MessageInfo& second) { return first.time < second.time; }
    /// Compare messages by time
    friend bool operator>(const MessageInfo& first, const MessageInfo& second) { return first.time > second.time; }
    /// Operator == to compare Message Info
    friend bool operator==(const MessageInfo& first, const MessageInfo& second)
    {
        return first.message == second.message && first.channelID == second.channelID && first.time == second.time &&
               first.msgID == second.msgID;
    }
};

class LiteMessageRepository : public IMessageRepository, public AbstractLiteRepository
{
public:
    LiteMessageRepository(std::shared_ptr<IAdapter>& adapter);
    void addMessages(std::vector<MessageInfo>& messages) override;

    void                     editMessages(std::vector<MessageInfo>& messages) override;
    std::vector<MessageInfo> getMessages(std::uint64_t channelID) override;
    std::vector<MessageInfo> getMessageSince(std::uint64_t channelID, std::uint64_t since) override;
    std::vector<MessageInfo> getMessageBefore(std::uint64_t channelID, std::uint64_t before, std::uint64_t amount) override;
    void                     removeMessages(std::vector<MessageInfo>& messages) override;
};

}  // namespace DataAccess
