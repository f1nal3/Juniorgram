#pragma once

#include <Network/Primitives.hpp>
#include <QWidget>
#include <vector>

#include "MessageWidget.hpp"
#include "ScrollArea.hpp"
#include "ReplyMessageWidget.hpp"

/**
 * @class ChatHistory
 * @brief Widget for showing messages in the chat
 */
class ChatHistory : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for chat history
     * @param parent Parent widget
     */
    explicit ChatHistory(QWidget* parent = nullptr);

    /**
     * @brief Adds a new message to history
     * @param message Message Text
     * @param utc Seconds since epoch
     * @param user Usenname
     */
    void addMessage(const QString& message = QString(), quint64 utc = 0, const QString& user = "You");
    void addMessage(const Network::MessageInfo& messageInfo);

    /**
     * @brief Add a new reply to history
     * @param reply ReplyWidget with information
     */
    void addReply(ReplyWidget* reply);

    /**
     * @brief Clears all chat
     */
    void clear();

    /**
     * @brief Delete specified message
     * @param userId Message user ID
     * @param messageId Message ID
     */
    void deleteMessage(uint64_t userId, uint64_t messageId);

protected:
    /**
     * @brief Handle resizes
     * @param event Event
     */
    void resizeEvent(QResizeEvent* event) override;

private Q_SLOTS:

    /**
     * @brief Resize visible messageboxes
     */
    void resizeVisible();
Q_SIGNALS:
    /// Message has been deleted/restored
    void messageChanged();
    /**
     * @brief Message has been added
     */
    void messageAdded();
    void createReplySignal(ReplyWidget*);

private:
    /**
     * @brief Updates layout if some changed size
     * @param beenResized Update from resize?
     */
    void updateLayout(bool beenResized = false);

    /**
     * @brief Finds visible messages
     * @return Left and right visible messages
     */
    [[nodiscard]] std::pair<int, int> findVisible() const;

private:
    bool                                                         _alreadyScrolling = false;
    std::int32_t                                                 _left             = -1;
    std::unique_ptr<ScrollArea>                                  _scrollArea;
    std::map<int32_t, std::unique_ptr<ReplyMessageWidget>>       _replyList;
    std::vector<std::unique_ptr<MessageWidget>>                  _messageList;
    std::vector<Network::MessageInfo>           _messages;
    std::uint64_t                                                _userId    = 0;
    std::uint64_t                                                _messageId = 0;
};
