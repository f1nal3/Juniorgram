#pragma once

#include <QResizeEvent>
#include <QWidget>
#include <memory>

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
    ReplyWidget(QWidget* parent, const Style::MessageWidget& st = st::defaultMessageWidget);

    /// Function to get username
    QString getUsername() const { return _username; };
    /// Function to get message
    QString getMessage() const { return _messageText; };
    /// Function to get messageId
    uint64_t getMessageId() const { return _messageId; };
    /// Function to get userId
    uint64_t getUserId() const { return _userId; };

    /// Expected height of Reply Widget
    int expectedHeight();
    /// Set reply
    void setReply(QString messageText, QString username, uint64_t messageId);
    /// Return message
    QString getMessageText() const { return _messageText; }

signals:
    /// Visibility has been changed
    void visibilityChanged(bool hidden);

protected:
    /// Event that is responsible for rendering the message
    void paintEvent(QPaintEvent* e) override;
    /// Event that is responsible for changing the size of the message
    void resizeEvent(QResizeEvent* e) override;
    /// Handle hide event
    void hideEvent(QHideEvent* e) override;
    /// Handle show event
    void showEvent(QShowEvent* e) override;

private:
    QString                     _messageText;
    QString                     _username;
    uint64_t                    _messageId;
    uint64_t                    _userId;
    std::unique_ptr<FlatButton> _closeReplyButton;
    const Style::MessageWidget& _st;
};
