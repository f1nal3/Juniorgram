#include "ChatWidget.hpp"

#include <QtEvents>
#include <utility>

#include "Application.hpp"
#include "ChatHistory.hpp"

ChatWidget::ChatWidget(std::string_view channelName, QWidget* parent) : QWidget(parent), _channelName(channelName)
{
    setContentsMargins(0, 0, 0, 0);

    _channelBar   = std::make_unique<ChannelBar>(this, QString::fromStdString(_channelName));
    _chatHistory  = std::make_unique<ChatHistory>(this);
    _textEdit     = std::make_unique<TextEdit>(this);
    _replyWidget  = std::make_unique<ReplyWidget>(this);
    _requestTimer = std::make_unique<QTimer>();
    connect(_requestTimer.get(), &QTimer::timeout, this, &ChatWidget::requestMessages);
    connect(_channelBar.get(), &ChannelBar::leaveChannelClick, this, [this]() {
        this->close();
        emit this->removeChannel();
    });
    /// Once in a second
    _requestTimer->start(1000);
    _replyWidget->hide();

    setMinimumWidth(st::chatWidgetMinWidth);
    connect(_chatHistory.get(), &ChatHistory::createReplySignal, this, &ChatWidget::setReply);
    connect(_textEdit.get(), &TextEdit::sendMessage, this, &ChatWidget::newMessage);

    connect(_replyWidget.get(), &ReplyWidget::visibilityChanged, [this](bool) { this->updateLayout(); });
    connect(_textEdit.get(), &TextEdit::textChanged, [this]() { this->updateLayout(); });

    connect(ReceiverManager::instance(), &ReceiverManager::onReplyHistoryAnswer, this, &ChatWidget::addReplies);
    connect(ReceiverManager::instance(), &ReceiverManager::onMessageHistoryAnswer, this, &ChatWidget::addMessages);
}

void ChatWidget::newMessage(const QString& messageText) const
{
    oApp->connectionManager()->storeMessage(messageText.toStdString(), _channelID);
    if (!_replyWidget->isHidden())
    {
        oApp->connectionManager()->storeReply(_replyWidget->getMessage().toStdString(), _channelID, _replyWidget->getMessageId());
        _replyWidget->close();
    }
}

void ChatWidget::addMessages(const std::vector<Models::MessageInfo>& messages) const
{
    if (messages.empty()) return;
    if (messages.front()._channelID != _channelID) return;  // It could be messages for other channel

    for (const auto& message : messages)
    {
        _chatHistory->addMessage(message);
    }
}

void ChatWidget::addReplies(const std::vector<Models::ReplyInfo>& replies) const
{
    if (replies.empty()) return;
    if (replies.front()._channelID != _channelID) return;  // It could be replies for other channel

    for (const auto& reply : replies)
    {
        _chatHistory->addReply(reply);
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

void ChatWidget::setReply(QString messageText, QString username, uint64_t messageId) const
{
    _replyWidget->setReply(std::move(messageText), std::move(username), messageId);
    _replyWidget->show();
}

void ChatWidget::updateLayout() const
{
    const auto& size = this->size();
    _replyWidget->setFixedWidth(size.width());  // Reply on whole width
    _channelBar->setFixedWidth(size.width());

    auto textEditExpectedHeightBefore = _textEdit->expectedHeight();  // Text edit size before resize
    _textEdit->resize(size.width(), textEditExpectedHeightBefore);
    auto textEditExpectedHeightAfter = _textEdit->expectedHeight();   // Text edit size after resize
    if (textEditExpectedHeightBefore != textEditExpectedHeightAfter)  // Need to resize again
        _textEdit->resize(size.width(), textEditExpectedHeightAfter);

    _chatHistory->resize(size.width(), size.height() - _textEdit->height() - _channelBar->height() -
                                           (_replyWidget->isHidden() ? 0 : _replyWidget->height()));

    _chatHistory->move(0, _channelBar->height());

    _textEdit->move(0, size.height() - _textEdit->height());
    _replyWidget->move(0, _textEdit->y() - _replyWidget->height());
}

void ChatWidget::resizeEvent(QResizeEvent*) { updateLayout(); }
