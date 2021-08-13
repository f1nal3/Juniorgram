#include "ChatHistory.hpp"

#include <QCoreApplication>
#include <QDebug>
#include <QPainter>
#include <QtEvents>

ChatHistory::ChatHistory(QWidget* parent) : QWidget(parent), _messageList()
{
    _messageList.reserve(1024);
    _scrollArea  = std::make_unique<ScrollArea>(this);
    auto history = std::make_unique<QWidget>(this);

    _scrollArea->setOwnedWidget(std::move(history));
    _scrollArea->setWidgetResizable(true);

    connect(_scrollArea.get(), SIGNAL(scrolled()), this, SLOT(resizeVisible()));
}

void ChatHistory::addReply(ReplyWidget* reply)
{
    auto history = _scrollArea->widget();
    auto replyMsg = new ReplyMessageWidget(history, reply->getMessage(), reply->getMessageId(), reply->getUsername());
    replyMsg->show();
    replyMsg->resize(history->width() - 25, replyMsg->expectedHeight());

    history->setMinimumHeight(history->minimumHeight() + replyMsg->expectedHeight() + 10);

    connect(replyMsg, &ReplyMessageWidget::geometryChanged, this, [=](int diff) {
            history->setMinimumHeight(history->minimumHeight() + diff);
            updateLayout(true);
    });

    _replyList.insert(std::make_pair(_messageList.size(), std::unique_ptr<ReplyMessageWidget>(replyMsg)));

    reply->close();
}

void ChatHistory::addMessage(const QString& message, quint64 utc, const QString& user)
{
    auto history = _scrollArea->widget();
    auto msg = new MessageWidget(history, message, _messageId++, _userId, utc, user);
    msg->show();
    msg->resize(history->width() - 25, msg->expectedHeight());
    _messageList.push_back(std::unique_ptr<MessageWidget>(msg));

    history->setMinimumHeight(history->minimumHeight() + msg->expectedHeight() + 10);

    connect(msg, &MessageWidget::geometryChanged, this, [=](int diff) {
        history->setMinimumHeight(history->minimumHeight() + diff);
        updateLayout(true);
    });

    connect(msg, &MessageWidget::createReplySignal, this, &ChatHistory::createReplySignal);

    updateLayout();

    _scrollArea->scrollToWidget(msg);

    emit messageAdded();
}

void ChatHistory::clear()
{
    _messageList.clear();
    _replyList.clear();
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
        auto it = _replyList.find(_left);
        y = _messageList[_left]->pos().y();
        if(it != _replyList.end())
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
        if(it != _replyList.rend())
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
        int topy = 0;
        auto it = _replyList.find(_left);
        topy    = _messageList[_left]->pos().y() + diff;
        if(it != _replyList.end())
        {
            topy = it->second->pos().y() + diff;
        }
        int bottomy = topy + height();
        if (_messageList.back()->geometry().top() <= bottomy) haveLast = true;
    }

    if (_left >= 0 && beenResized && !haveLast)
    {
        int newy = 0;
        auto it = _replyList.find(_left);
        newy = _messageList[_left]->pos().y();
        if(it->first == _left)
        {
            newy = it->second->pos().y();
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
    bool bHeight;
    bool bWidth;

    auto it = _replyList.find(left);
    for (int index = left; index <= right; index++)
    {
        it = _replyList.find(index);
        if(it != _replyList.end())
        {
            bHeight = it->second->expectedHeight() != it->second->height();
            bWidth = it->second->width() != width;
            resized |= bHeight || bWidth;
            if(bWidth || bHeight) it->second->resize(width, it->second->expectedHeight());
        }

        auto&      msg     = _messageList[index];
        bHeight = msg->expectedHeight() != msg->height();
        bWidth  = msg->width() != width;
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
        auto it = _replyList.find((right - left) / 2);
        middle    = _messageList[(right - left) / 2]->pos().y();
        if(it != _replyList.end())
        {
            middle = it->second->pos().y();
        }
        auto isBetween = [](int p, int t, int b) { return p >= t && p <= b; };
        while (!isBetween(middle, top, bottom) && (right - left) > 1)
        {
            if (middle < top) left = (right + left) / 2;
            if (middle > top) right = (right + left) / 2;
            it = _replyList.find((right - left) / 2);
            middle = _messageList[(right - left) / 2]->pos().y();
            if(it != _replyList.end())
            {
                middle = it->second->pos().y();
            }
        }
    }
    int middle = (right + left) / 2;
    int index  = middle;
    int size   = int(_messageList.size());

    int sizeHeight = 0;

    auto it = _replyList.find(index);
    sizeHeight = _messageList[index]->pos().y();
    if(it != _replyList.end())
    {
        sizeHeight = it->second->pos().y();
    }

    while (sizeHeight < bottom && index < size - 1)
    {
        it = _replyList.find(index);
        sizeHeight = _messageList[index]->pos().y();
        if(it != _replyList.end())
        {
            sizeHeight = it->second->pos().y();
        }
        index++;
    }
    right = index;
    index = middle;

    it = _replyList.find(index);
    sizeHeight = _messageList[index]->pos().y() + _messageList[index]->height();
    if(it != _replyList.end())
    {
        sizeHeight = it->second->pos().y() + it->second->height();
    }

    while(sizeHeight > top && index > 0)
    {
        it = _replyList.find(index);
        sizeHeight = _messageList[index]->pos().y() + _messageList[index]->height();
        if(it != _replyList.end())
        {
            sizeHeight = it->second->pos().y() + it->second->height();
        }
        index--;
    }

    left = index;
    return {left, right};
}
