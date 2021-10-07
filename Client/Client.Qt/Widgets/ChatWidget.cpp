#include "ChatWidget.hpp"

#include <QtDebug>
#include <QtEvents>
#include <utility>

#include "Application.hpp"
#include "ChatHistory.hpp"

ChatWidget::ChatWidget(QWidget* parent) : QWidget(parent)
{
    setContentsMargins(0, 0, 0, 0);
    _chatHistory  = std::make_unique<ChatHistory>(this);
    _textEdit     = std::make_unique<TextEdit>(this);
    _replyWidget  = std::make_unique<ReplyWidget>(this);
    _requestTimer = std::make_unique<QTimer>();

    connect(_requestTimer.get(), &QTimer::timeout, this, &ChatWidget::requestMessages);

    /// Once in a second
    _requestTimer->start(1000);
    _replyWidget->hide();

    setMinimumWidth(st::chatWidgetMinWidth);
    connect(_chatHistory.get(), &ChatHistory::createReplySignal, this, &ChatWidget::setReply);
    connect(_textEdit.get(), &TextEdit::sendMessage, this, &ChatWidget::newMessage);

    connect(_replyWidget.get(), &ReplyWidget::visibilityChanged, [=](bool) { layout(); });
    connect(_textEdit.get(), &TextEdit::textChanged, [=]() { layout(); });

    connect(ReceiverManager::instance(), &ReceiverManager::onReplyHistoryAnswer, this, &ChatWidget::addReplies);
    connect(ReceiverManager::instance(), &ReceiverManager::onMessageHistoryAnswer, this, &ChatWidget::addMessages);
}

void ChatWidget::newMessage(const QString& messageText)
{
    oApp->connectionManager()->storeMessage(messageText.toStdString(), _channelID);
    if (!_replyWidget->isHidden())
    {
        oApp->connectionManager()->storeReply(_replyWidget->getMessage().toStdString(), _channelID, _replyWidget->getMessageId());
        _replyWidget->close();
    }
}

void ChatWidget::addMessages(const std::vector<Network::MessageInfo>& messages)
{
    for (const auto& message : messages)
    {
        if (message.channelID == _channelID)
        {
            _chatHistory->addMessage(message);
        }
    }
}

void ChatWidget::addReplies(const std::vector<Network::ReplyInfo>& replies)
{
    for (const auto& reply : replies)
    {
        if (reply.channelID == _channelID)
        {
            _chatHistory->addReply(reply);
        }
    }
}

void ChatWidget::requestMessages() const
{
    if (oApp->connectionManager()->isConnected())
    {
        oApp->connectionManager()->askForMessageHistory(_channelID);
        oApp->connectionManager()->askForReplyHistory(_channelID);
    }
}

void ChatWidget::setReply(QString messageText, QString username, uint64_t messageId)
{
    _replyWidget->setReply(std::move(messageText), std::move(username), messageId);
    _replyWidget->show();
}

void ChatWidget::layout()
{
    const auto& size = this->size();
    _replyWidget->setFixedWidth(size.width());
    auto textEditExpectedHeightBefore = _textEdit->expectedHeight();
    _textEdit->resize(size.width(), textEditExpectedHeightBefore);
    auto textEditExpectedHeightAfter = _textEdit->expectedHeight();
    if (textEditExpectedHeightBefore != textEditExpectedHeightAfter) _textEdit->resize(size.width(), textEditExpectedHeightAfter);
    _chatHistory->resize(size.width(), size.height() - _textEdit->height() - (_replyWidget->isHidden() ? 0 : _replyWidget->height()));
    _textEdit->move(0, size.height() - _textEdit->height());
    _replyWidget->move(0, _textEdit->y() - _replyWidget->height());
}

void ChatWidget::resizeEvent(QResizeEvent*) { layout(); }
