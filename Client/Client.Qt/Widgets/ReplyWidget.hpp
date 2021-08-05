#pragma once

#include <QWidget>
#include <memory>

#include "FlatButton.hpp"
#include "InputFields.hpp"

class ReplyWidget : public QWidget
{
    Q_OBJECT

public:
    ReplyWidget(QString message, QString username, uint64_t messageId, const Style::MessageWidget& st = st::defaultMessageWidget);

public: 
    int expectedHeight();

public slots:
    void closeReply();

signals:
    //void geometryChanged(int);
    void createReplySignal(ReplyWidget*);

private:
    QString _messageText;
    QString _username;
    uint64_t _messageId;
    std::unique_ptr<FlatButton> _closeReplyButton;
    std::unique_ptr<FlatTextEdit> _fmtMessageText;
    const Style::MessageWidget& _st;
};

