#include "ChatWidget.hpp"

#include "ChatHistory.hpp"
#include "ConnectionManager.hpp"

ChatWidget::ChatWidget(QWidget* parent) : QWidget(parent)
{
    setContentsMargins(0, 0, 0, 0);
    _mainChatLayout = std::make_unique<QVBoxLayout>(this);
    _chatHistory    = std::make_unique<ChatHistory>(this);
    _textEdit       = std::make_unique<TextEdit>(_chatHistory.get());

    _mainChatLayout->setContentsMargins(0, 0, 0, 0);
    _mainChatLayout->setSpacing(0);
    _mainChatLayout->addWidget(_chatHistory.get(), 85);
    _mainChatLayout->addWidget(_textEdit.get(), 15);

    setLayout(_mainChatLayout.get());

    setMinimumWidth(Style::valueDPIScale(400));
    connect(_textEdit.get(), SIGNAL(sendMessageSignal(QString)), this, SLOT(newMessage(QString)));
    connect(_chatHistory.get(), &ChatHistory::createReplySignal, this, &ChatWidget::addReplyWidget);
}

void ChatWidget::newMessage(const QString& messageText) 
{
    ConnectionManager::getClient().storeMessage(messageText.toStdString(), _channelID);
    if(this->findChild<ReplyWidget*>())
    {
        _chatHistory->addMessage(messageText, QDateTime::currentSecsSinceEpoch(), _replyWidget);
    }
    else
    {
        _chatHistory->addMessage(messageText, QDateTime::currentSecsSinceEpoch());
    }
}

void ChatWidget::newMessage(const QString& messageText, const QString& userNameMessage)
{
    if(this->findChild<ReplyWidget*>())
    {
        _chatHistory->addMessage(messageText, QDateTime::currentSecsSinceEpoch(), _replyWidget, userNameMessage);
    }
    else
    {
        _chatHistory->addMessage(messageText, QDateTime::currentSecsSinceEpoch(), nullptr, userNameMessage);
    }
}

void ChatWidget::addReplyWidget(ReplyWidget* reply)
{
    if(this->findChild<ReplyWidget*>())
    {
        _replyWidget->close();
    }

     _replyWidget = reply;
    _mainChatLayout->insertWidget(1, _replyWidget, 12);
}
