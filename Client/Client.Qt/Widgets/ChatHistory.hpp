#pragma once

#include <QWidget>
#include <vector>

#include "MessageWidget.hpp"
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
     * @param message Message Text
     * @param utc Seconds since epoch
     * @param user Usenname
     */
    void addMessage(const QString& message = QString(), quint64 utc = 0, const QString& user = "You");
    /**
     * @brief Clears all chat
     */
    void clear();
    void deleteMessage(int userId, int messageId);

protected:
    void resizeEvent(QResizeEvent* event) override;

private Q_SLOTS:

    /**
     * @brief Resize visible messageboxes
     */
    void resizeVisible();
Q_SIGNALS:
    /**
     * @brief Message has been deleted/restored
     */
    void messageChanged();
    /**
     * @brief Message has been added
     */
    void messageAdded();

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
    bool                                        _alreadyScrolling = false;
    std::int32_t                                _left = -1;
    std::unique_ptr<ScrollArea>                 _scrollArea;
    std::vector<std::unique_ptr<MessageWidget>> _messageList;
    uint64_t                                     _userId = 0;
    uint64_t                                     _messageId = 0;
};
