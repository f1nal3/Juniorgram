#include "ChatHistory.hpp"

#include <QCoreApplication>
#include <QDebug>
#include <QPainter>
#include <QtEvents>

ChatHistory::ChatHistory(QWidget* parent) : QWidget(parent), _messageList()
{
    _messageList.reserve(1024);
    _messages.reserve(1024);
    _scrollArea  = std::make_unique<ScrollArea>(this);
    auto history = std::make_unique<QWidget>(this);

    _scrollArea->setOwnedWidget(std::move(history));
    _scrollArea->setWidgetResizable(true);

    connect(_scrollArea.get(), SIGNAL(scrolled()), this, SLOT(resizeVisible()));
}

void ChatHistory::clear()
{
    _messageList.clear();
    _replyList.clear();
}

void ChatHistory::addReply(const Models::ReplyInfo& replyInfo)
{
    auto copy = std::find(_replies.begin(), _replies.end(), replyInfo);
    if (copy != _replies.end())
    {
        return;
    }

    auto history = _scrollArea->widget();

    auto replyMsg = new ReplyMessageWidget(history, QString::fromStdString(replyInfo.message), replyInfo.msgID,
                                           QString::fromStdString(replyInfo.userLogin), replyInfo.senderID);

    replyMsg->show();
    replyMsg->resize(history->width() - 25, replyMsg->expectedHeight());
    history->setMinimumHeight(history->minimumHeight() + replyMsg->expectedHeight() + 10);

    _replyList.insert(std::make_pair(int32_t(replyInfo.msgIdOwner), std::unique_ptr<ReplyMessageWidget>(replyMsg)));
    _replies.push_back(replyInfo);

    connect(replyMsg, &ReplyMessageWidget::geometryChanged, this, [=](int diff) {
        history->setMinimumHeight(history->minimumHeight() + diff);
        updateLayout(true);
    });

    updateLayout();
}

void ChatHistory::addMessage(const Models::MessageInfo& messageInfo)
{
    auto copy = std::find(_messages.begin(), _messages.end(), messageInfo);

    auto history = _scrollArea->widget();

    if (copy != _messages.end())
    {
        if (copy->reactions != messageInfo.reactions)
        {
            auto widgetCopy = std::find_if(_messageList.begin(), _messageList.end(), [messageInfo](const std::unique_ptr<MessageWidget>& mw)
                                        { return mw->isTheMessage(messageInfo.msgID, messageInfo.senderID); });

            history->setMinimumHeight(history->minimumHeight() - (*widgetCopy)->height() - 10);

            _messages.erase(copy);
            _messageList.erase(widgetCopy);
        }
        else
        {
            return;
        }
    }

    auto msg = new MessageWidget(history, QString::fromStdString(messageInfo.message), messageInfo.senderID, messageInfo.msgID,
                                 messageInfo.time, QString::fromStdString(messageInfo.userLogin));

    msg->setReactionMap(messageInfo.reactions);
    msg->show();
    msg->resize(history->width() - 25, msg->expectedHeight());
    history->setMinimumHeight(history->minimumHeight() + msg->expectedHeight() + 10);

    _messageList.push_back(std::unique_ptr<MessageWidget>(msg));
    _messages.push_back(messageInfo);

    std::sort(_messageList.begin(), _messageList.end(), [](const std::unique_ptr<MessageWidget>& lhs, const std::unique_ptr<MessageWidget>& rhs)
        { return *lhs < *rhs;  });

    connect(msg, &MessageWidget::createReplySignal, this, &ChatHistory::createReplySignal);

    connect(msg, &MessageWidget::geometryChanged, this, [=](int diff) {
        history->setMinimumHeight(history->minimumHeight() + diff);
        updateLayout(true);
    });

    updateLayout();

    _scrollArea->scrollToWidget(msg);

    emit messageAdded();
}

void ChatHistory::deleteMessage(const uint64_t userId, const uint64_t messageId)
{
    for (auto& msg : _messageList)
    {
        if (msg->isTheMessage(messageId, userId)) msg->onDelete();
    }
}

void ChatHistory::resizeEvent(QResizeEvent* event)
{
    _scrollArea->resize(width(), height());

    if (event->oldSize().width() != event->size().width())
    {
        resizeVisible();
    }

    if (event->oldSize().height() != event->size().height() && !_alreadyScrolling)
    {
        updateLayout();
    }
}

void ChatHistory::updateLayout(bool beenResized)
{
    if (_messageList.empty()) return;
    auto bottom = _scrollArea->scrollHeight();

    int diff = _scrollArea->scrollTop();
    int y    = diff;

    if (_left >= 0)
    {
        auto it = _replyList.find(int(_messageList[_left]->getMessageID()));
        y       = _messageList[_left]->pos().y();
        if (it != _replyList.end())
        {
            y = it->second->pos().y();
        }
        diff -= y;
    }
    auto it = _replyList.rbegin();
    for (int i = int(_messageList.size()) - 1; i >= 0; i--)
    {
        _messageList[i]->move(0, bottom - _messageList[i]->height());
        bottom -= _messageList[i]->height() + 10;
        if (it != _replyList.rend() && it->first == int(_messageList[i]->getMessageID()))
        {
            it->second->move(0, bottom - it->second->height());
            bottom -= it->second->height() + 10;
            it++;
        }
    }

    bool haveLast = false;

    auto [newleft, newright] = findVisible();

    if (newright == int(_messageList.size()) - 1 && _left >= 0)
    {
        int  topy = 0;
        auto iter = _replyList.find(int(_messageList[_left]->getMessageID()));
        topy      = _messageList[_left]->pos().y() + diff;
        if (iter != _replyList.end())
        {
            topy = iter->second->pos().y() + diff;
        }
        int bottomy = topy + height();
        if (_messageList.back()->geometry().top() <= bottomy) haveLast = true;
    }

    if (_left >= 0 && beenResized && !haveLast)
    {
        int  newy = 0;
        auto iter = _replyList.find(int(_messageList[_left]->getMessageID()));
        newy      = _messageList[_left]->pos().y();
        if (iter != _replyList.end())
        {
            newy = iter->second->pos().y();
        }
        int top           = newy + diff;
        _alreadyScrolling = true;
        _scrollArea->scrollToY(top);
        _alreadyScrolling = false;
    }
    if (haveLast && beenResized)
    {
        _scrollArea->scrollToY(_messageList.back()->pos().y());
    }
}

void ChatHistory::resizeVisible()
{
    if (_alreadyScrolling) return;
    if (_messageList.empty())
    {
        _left = -1;
        return;
    }
    const auto [left, right]   = findVisible();
    const std::int32_t width   = this->width() - 25;
    bool               resized = false;
    bool               bHeight;
    bool               bWidth;

    auto it = _replyList.find(int(_messageList[left]->getMessageID()));
    for (int index = left; index <= right; index++)
    {
        it = _replyList.find(int(_messageList[index]->getMessageID()));
        if (it != _replyList.end())
        {
            bHeight = it->second->expectedHeight() != it->second->height();
            bWidth  = it->second->width() != width;
            resized |= bHeight || bWidth;
            if (bWidth || bHeight) it->second->resize(width, it->second->expectedHeight());
        }

        auto& msg = _messageList[index];
        bHeight   = msg->expectedHeight() != msg->height();
        bWidth    = msg->width() != width;
        resized |= bHeight || bWidth;
        if (bWidth || bHeight) msg->resize(width, msg->expectedHeight());
        msg->setIndex(index);
    }
    if (left >= 0 && left + 1 < int(_messageList.size())) _left = left + 1;
    updateLayout(resized);
}

std::pair<int, int> ChatHistory::findVisible() const
{
    int left   = 0;
    int right  = int(_messageList.size());
    int top    = _scrollArea->scrollTop();
    int bottom = top + _scrollArea->height();
    {
        int  middle = 0;
        auto it     = _replyList.find(int(_messageList[(right - left) / 2]->getMessageID()));
        middle      = _messageList[(right - left) / 2]->pos().y();
        if (it != _replyList.end())
        {
            middle = it->second->pos().y();
        }
        auto isBetween = [](int p, int t, int b) { return p >= t && p <= b; };
        while (!isBetween(middle, top, bottom) && (right - left) > 1)
        {
            if (middle < top) left = (right + left) / 2;
            if (middle > top) right = (right + left) / 2;
            it     = _replyList.find(int(_messageList[(right - left) / 2]->getMessageID()));
            middle = _messageList[(right - left) / 2]->pos().y();
            if (it != _replyList.end())
            {
                middle = it->second->pos().y();
            }
        }
    }
    int middle = (right + left) / 2;
    int index  = middle;
    int size   = int(_messageList.size());

    int sizeHeight = 0;

    auto it    = _replyList.find(int(_messageList[index]->getMessageID()));
    sizeHeight = int(_messageList[index]->pos().y());
    if (it != _replyList.end())
    {
        sizeHeight = int(it->second->pos().y());
    }

    while (sizeHeight < bottom && index < size - 1)
    {
        it         = _replyList.find(int(_messageList[index]->getMessageID()));
        sizeHeight = int(_messageList[index]->pos().y());
        if (it != _replyList.end())
        {
            sizeHeight = int(it->second->pos().y());
        }
        index++;
    }
    right = index;
    index = int(middle);

    it         = _replyList.find(int(_messageList[index]->getMessageID()));
    sizeHeight = int(_messageList[index]->pos().y() + _messageList[index]->height());
    if (it != _replyList.end())
    {
        sizeHeight = int(it->second->pos().y() + it->second->height());
    }

    while (sizeHeight > top && index > 0)
    {
        it         = _replyList.find(int(_messageList[index]->getMessageID()));
        sizeHeight = int(_messageList[index]->pos().y() + _messageList[index]->height());
        if (it != _replyList.end())
        {
            sizeHeight = int(it->second->pos().y() + it->second->height());
        }
        index--;
    }

    left = index;
    return {left, right};
}
