#pragma once

#include <Models/Primitives.hpp>
#include <QWidget>
#include <vector>

#include "MessageWidget.hpp"
#include "ReplyMessageWidget.hpp"
#include "ScrollArea.hpp"

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
     * @param Network::MessageInfo& messageInfo
     */
    void addMessage(const Base::Models::MessageInfo& messageInfo);

    /**
     * @brief Add a new reply to history
     * @param Network::ReplyInfo& replyInfo
     */
    void addReply(const Base::Models::ReplyInfo& replyInfo);

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
    /// Message has been added
    void messageAdded();
    /// This message is replied
    void createReplySignal(QString messageText, QString username, uint64_t messageId);

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
    bool                                                   _alreadyScrolling = false;
    std::int32_t                                           _left             = -1;
    std::unique_ptr<ScrollArea>                            _scrollArea;
    std::map<int32_t, std::unique_ptr<ReplyMessageWidget>> _replyList;
    std::vector<std::unique_ptr<MessageWidget>>            _messageList;
    std::vector<Base::Models::MessageInfo>                 _messages;
    std::vector<Base::Models::ReplyInfo>                   _replies;
    std::uint64_t                                          _userId    = 0;
    std::uint64_t                                          _messageId = 0;
};
