#include "ChatWidget.hpp"

#include "ChatHistory.hpp"
#include "ConnectionManager.hpp"

#include <iostream>

ChatWidget::ChatWidget(QWidget* parent) : QWidget(parent)
{
    setContentsMargins(0, 0, 0, 0);
    _mainChatLayout = std::make_unique<QVBoxLayout>(this);
    _chatHistory    = std::make_unique<ChatHistory>();
    _textEdit       = std::make_unique<TextEdit>(_chatHistory.get());

    _mainChatLayout->setContentsMargins(0, 0, 0, 0);
    _mainChatLayout->setSpacing(0);
    _mainChatLayout->addWidget(_chatHistory.get(), 85);
    _mainChatLayout->addWidget(_textEdit.get(), 15);

    setMinimumWidth(Style::valueDPIScale(400));
    connect(_textEdit.get(), SIGNAL(sendMessageSignal(QString)), this, SLOT(newMessage(QString)));
    connect(_chatHistory.get(), &ChatHistory::createReplySignal, this, &ChatWidget::addReplyWidget);
}

void ChatWidget::newMessage(const QString& messageText) 
{
    ConnectionManager::getClient().storeMessage(messageText.toStdString(), _channelID);
    _chatHistory->addMessage(messageText, QDateTime::currentSecsSinceEpoch()); 
}

void ChatWidget::newMessage(const QString& messageText, const QString& userNameMessage)
{
    _chatHistory->addMessage(messageText, QDateTime::currentSecsSinceEpoch(), userNameMessage);
}

void ChatWidget::addReplyWidget(ReplyWidget* reply)
{
    if(_replyWidget != nullptr)
    {
        _replyWidget->close();
    }
    /*
    if(this->findChild<ReplyWidget*>())
    {
        std::cout << "YES IS WORK" << std::endl;
    }
    */
     _replyWidget = reply;
    _mainChatLayout->insertWidget(1, _replyWidget, 20);

    std::cout << _replyWidget << std::endl;
}
