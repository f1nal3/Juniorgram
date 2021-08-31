#pragma once

#include <QWidget>
#include <memory>
#include <QResizeEvent>

#include "Buttons.hpp"
#include "InputFields.hpp"

/**
 * @class ReplyWidget
 * @brief Class for display current reply above text edit widget
 */
class ReplyWidget : public QWidget
{
    Q_OBJECT

public:
    /// Constructor for reply widget
    ReplyWidget(QString message, QString username, uint64_t messageId, uint64_t userId = uint64_t(),
                const Style::MessageWidget& st = st::defaultMessageWidget);

public:
    /// Function to get username
    QString getUsername() const { return _username; };
    /// Function to get message
    QString getMessage() const { return _messageText; };
    /// Function to get messageId
    uint64_t getMessageId() const { return _messageId; };
    /// Function to get userId
    uint64_t getUserId() const { return _userId; };
    /// Possible height of message widget
    int expectedHeight();

public slots:
    /// Slot for close reply above text edit widget after sending message
    void closeReply();

protected:
    /// Event that is responsible for rendering the message
    void paintEvent(QPaintEvent* e) override;
    /// Event that is responsible for changing the size of the message
    void resizeEvent(QResizeEvent* e) override;

private:
    QString _messageText;
    QString _username;
    uint64_t _messageId;
    uint64_t _userId;
    std::unique_ptr<FlatButton> _closeReplyButton;
    std::unique_ptr<FlatTextEdit> _fmtMessageText;
    const Style::MessageWidget& _st;
};

