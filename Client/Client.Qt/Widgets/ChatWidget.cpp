#include "ChatWidget.hpp"

#include "Application.hpp"
#include "ChatHistory.hpp"

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

    setMinimumWidth(st::chatWidgetMinWidth);
    connect(_textEdit.get(), &TextEdit::sendMessage, this, &ChatWidget::newMessage);
    connect(ReceiverManager::instance(), &ReceiverManager::onMessageHistoryAnswer, this, &ChatWidget::addMessages);
}

void ChatWidget::newMessage(const QString& messageText)
{
    oApp->connectionManager()->storeMessage(messageText.toStdString(), _channelID);
    _chatHistory->addMessage(messageText, QDateTime::currentSecsSinceEpoch());
}

void ChatWidget::newMessageA(const QString& messageText, const QString& username)
{
    _chatHistory->addMessage(messageText, QDateTime::currentSecsSinceEpoch(), username);
}

void ChatWidget::addMessages(const std::vector<Network::MessageInfo>&)
{
    /// TODO: implement it
}