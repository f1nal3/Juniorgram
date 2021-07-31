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
}

void ChatWidget::newMessage(const QString& messageText) 
{
    const std::uint64_t userID = 40;
    ConnectionManager::getClient().storeMessage(messageText.toStdString(), userID, _channelID);
    _chatHistory->addMessage(messageText, QDateTime::currentSecsSinceEpoch()); 
}

void ChatWidget::newMessage(const QString& messageText, const QString& userNameMessage)
{
    _chatHistory->addMessage(messageText, QDateTime::currentSecsSinceEpoch(), userNameMessage);
}
