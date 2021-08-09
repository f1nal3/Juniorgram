#pragma once

#include <QWidget>
#include <memory>
#include <QResizeEvent>

#include "Buttons.hpp"
#include "InputFields.hpp"

class ReplyWidget : public QWidget
{
    Q_OBJECT

public:
    ReplyWidget(QString message, QString username, uint64_t messageId, const Style::MessageWidget& st = st::defaultMessageWidget);

public: 
    QString getUsername() const { return _username; };
    QString getMessage() const { return _messageText; };
    uint64_t getMessageId() const { return _messageId; };
    int expectedHeight();

public slots:
    void closeReply();

protected:
    void paintEvent(QPaintEvent* e) override;
    void resizeEvent(QResizeEvent* e) override;

private:
    QString _messageText;
    QString _username;
    uint64_t _messageId;
    std::unique_ptr<FlatButton> _closeReplyButton;
    std::unique_ptr<FlatTextEdit> _fmtMessageText;
    const Style::MessageWidget& _st;
};

