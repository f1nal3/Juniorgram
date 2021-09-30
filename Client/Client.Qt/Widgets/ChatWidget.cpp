#include "ChatWidget.hpp"

#include <QDebug>

#include "Application.hpp"
#include "ChatHistory.hpp"

namespace
{
    /*
     * @brief   Removes redundant whitespaces
     * @param   Input string as input
     * @details Removes whitespaces at the beginning and the end of input string, replaces whitespace
     *          sequences with a single space or newline (if sequence contains a newline)
     */
    std::string removeSpaces(const std::string& input)
    {
        std::string result;
        result.reserve(input.size());

        auto sequenceStart = std::find_if_not(input.cbegin(), input.cend(), std::isspace);
        auto sequenceEnd = sequenceStart;

        while (sequenceStart < input.end())
        {
            bool spaceSequence = std::isspace(*sequenceStart);

            if (spaceSequence)
            {
                char singleSpace = ' ';

                for (; (sequenceEnd < input.end()) && std::isspace(*sequenceEnd); ++sequenceEnd)
                {
                    if (*sequenceEnd == '\n')
                    {
                        singleSpace = '\n';
                    }
                }

                result.push_back(singleSpace);
            }
            else
            {
                sequenceEnd = std::find_if(sequenceStart, input.cend(), std::isspace);
                result.append(sequenceStart, sequenceEnd);
            }

            sequenceStart = sequenceEnd;
        }

        if (!result.empty() && std::isspace(result.back()))
        {
            result.pop_back();
        }

        return result;
    }
}

ChatWidget::ChatWidget(QWidget* parent) : QWidget(parent)
{
    setContentsMargins(0, 0, 0, 0);
    _mainChatLayout = std::make_unique<QVBoxLayout>(this);
    _chatHistory    = std::make_unique<ChatHistory>(this);
    _textEdit       = std::make_unique<TextEdit>(_chatHistory.get());
    _requestTimer   = std::make_unique<QTimer>();

    _mainChatLayout->setContentsMargins(0, 0, 0, 0);
    _mainChatLayout->setSpacing(0);
    _mainChatLayout->addWidget(_chatHistory.get(), 85);
    _mainChatLayout->addWidget(_textEdit.get(), 15);

    setLayout(_mainChatLayout.get());

    connect(_requestTimer.get(), &QTimer::timeout, this, &ChatWidget::requestMessages);
    connect(_requestTimer.get(), &QTimer::timeout, this, &ChatWidget::requestReplies);

    /// Once in a second
    _requestTimer->start(1000);

    setMinimumWidth(st::chatWidgetMinWidth);
    connect(_chatHistory.get(), &ChatHistory::createReplySignal, this, &ChatWidget::addReplyWidget);
    connect(_textEdit.get(), &TextEdit::sendMessage, this, &ChatWidget::newMessage);
    connect(ReceiverManager::instance(), &ReceiverManager::onReplyHistoryAnswer, this, &ChatWidget::addReplies);
    connect(ReceiverManager::instance(), &ReceiverManager::onMessageHistoryAnswer, this, &ChatWidget::addMessages);
}

void ChatWidget::newMessage(const QString& messageText)
{
    std::string formattedMessage = removeSpaces(messageText.toStdString());

    oApp->connectionManager()->storeMessage(formattedMessage, _channelID);

    if (this->findChild<ReplyWidget*>())
    {
        std::string formattedReply = removeSpaces(_replyWidget->getMessage().toStdString());

        oApp->connectionManager()->storeReply(formattedReply, _channelID, _replyWidget->getMessageId());
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
    for(const auto& reply : replies)
    {
        if (reply.channelID == _channelID)
        {
            _chatHistory->addReply(reply);
        }
    }
}

void ChatWidget::requestMessages()
{
    if (oApp->connectionManager()->isConnected())
    {
        oApp->connectionManager()->askForMessageHistory(_channelID);
    }
}

void ChatWidget::requestReplies()
{
    if(oApp->connectionManager()->isConnected())
    {
        oApp->connectionManager()->askForReplyHistory(_channelID);
    }
}

void ChatWidget::addReplyWidget(ReplyWidget* reply)
{
    if(this->findChild<ReplyWidget*>())
    {
        _replyWidget->close();
    }

     _replyWidget = reply;
    _mainChatLayout->insertWidget(1, _replyWidget, 15);
}
