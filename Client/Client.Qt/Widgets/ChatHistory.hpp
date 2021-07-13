#pragma once

#include <QWidget>
#include <vector>

#include "MessageWidget.hpp"
#include "ScrollArea.hpp"

class ChatHistory : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor of chat history
     * @param parent Parent widget
     */
    explicit ChatHistory(QWidget* parent = nullptr);

    void addMessage(const QString& message, quint64 utc, const QString& user = "You");
    void clear();

    explicit operator QWidget*() { return _scrollArea->widget(); }

protected:
    void resizeEvent(QResizeEvent* event) override;

private Q_SLOTS:

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
    void updateLayout(bool beenResized = false);

    [[nodiscard]] std::pair<int, int> findVisible() const;

private:
    bool                                        _alreadyScrolling = false;
    int                                         _left             = -1;
    int                                         _right            = -1;
    std::unique_ptr<ScrollArea>                 _scrollArea;
    std::vector<std::unique_ptr<MessageWidget>> _messageList;
};
