#include "ChatHistory.hpp"

#include <iostream>

#include <QCoreApplication>
#include <QDebug>
#include <QPainter>
#include <QtEvents>

ChatHistory::ChatHistory(QWidget* parent) : QWidget(parent), _messageAndReplyList()
{
    _messageAndReplyList.reserve(1024);
    _scrollArea  = std::make_unique<ScrollArea>(this);
    auto history = std::make_unique<QWidget>(this);

    _scrollArea->setOwnedWidget(std::move(history));
    _scrollArea->setWidgetResizable(true);

    connect(_scrollArea.get(), SIGNAL(scrolled()), this, SLOT(resizeVisible()));
}

void ChatHistory::addMessage(const QString& message, quint64 utc, ReplyWidget* reply, const QString& user)
{
    auto history = _scrollArea->widget();
    if(reply != nullptr)
    {
        auto replyMsg = new ReplyMessageWidget(history, reply->getMessage(), reply->getMessageId(), reply->getUsername());
        auto msg = new MessageWidget(history, message, _messageId++, _userId, utc, user);
        replyMsg->show();
        replyMsg->resize(history->width() - 25, replyMsg->expectedHeight());
        msg->show();
        msg->resize(history->width() - 25, msg->expectedHeight());

        history->setMinimumHeight(history->minimumHeight() + replyMsg->expectedHeight() + 10);

        connect(replyMsg, &ReplyMessageWidget::geometryChanged, this, [=](int diff) {
            history->setMinimumHeight(history->minimumHeight() + diff);
            updateLayout(true);
        });


        history->setMinimumHeight(history->minimumHeight() + msg->expectedHeight() + 10);


        connect(msg, &MessageWidget::geometryChanged, this, [=](int diff) {
            history->setMinimumHeight(history->minimumHeight() + diff);
            updateLayout(true);
        });

        connect(msg, &MessageWidget::createReplySignal, this, &ChatHistory::createReplySignal);

        _messageAndReplyList.push_back(std::make_pair(std::unique_ptr<MessageWidget>(nullptr), std::unique_ptr<ReplyMessageWidget>(replyMsg)));
        _messageAndReplyList.push_back(std::make_pair(std::unique_ptr<MessageWidget>(msg), std::unique_ptr<ReplyMessageWidget>(nullptr)));

        updateLayout();

        _scrollArea->scrollToWidget(msg);

        _replyCount++;

        reply->close();
    }
    else
    {
        auto msg = new MessageWidget(history, message, _messageId++, _userId, utc, user);
        msg->show();
        msg->resize(history->width() - 25, msg->expectedHeight());
        _messageAndReplyList.push_back(std::make_pair(std::unique_ptr<MessageWidget>(msg), std::unique_ptr<ReplyMessageWidget>(nullptr)));

         history->setMinimumHeight(history->minimumHeight() + msg->expectedHeight() + 10);

        connect(msg, &MessageWidget::geometryChanged, this, [=](int diff) {
            history->setMinimumHeight(history->minimumHeight() + diff);
            updateLayout(true);
        });

        connect(msg, &MessageWidget::createReplySignal, this, &ChatHistory::createReplySignal);

        updateLayout();

        _scrollArea->scrollToWidget(msg);
    }

    emit messageAdded();
}

void ChatHistory::clear() { _messageAndReplyList.clear(); }

void ChatHistory::deleteMessage(const uint64_t userId, const uint64_t messageId)
{
    for (auto& msg : _messageAndReplyList)
    {
        if (msg.first->isTheMessage(messageId, userId)) msg.first->onDelete();
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
    if (_messageAndReplyList.empty()) return;
    auto bottom = _scrollArea->scrollHeight();

    int diff = _scrollArea->scrollTop();
    int y    = diff;

    if (_left >= 0)
    {
        if(_messageAndReplyList[_left].second != nullptr)
        {
            y = _messageAndReplyList[_left].second->pos().y();
        }
        else
        {
            y = _messageAndReplyList[_left].first->pos().y();
        }
        diff -= y;
    }
    for (int i = int(_messageAndReplyList.size()) - 1; i >= 0; i--)
    {
        if(_messageAndReplyList[i].second != nullptr)
        {
            _messageAndReplyList[i].second->move(0, bottom - _messageAndReplyList[i].second->height());
            bottom -= _messageAndReplyList[i].second->height() + 10;
        }
        else
        {
            _messageAndReplyList[i].first->move(0, bottom - _messageAndReplyList[i].first->height());
            bottom -= _messageAndReplyList[i].first->height() + 10;
        }
    }

    bool haveLast = false;

    auto [newleft, newright] = findVisible();

    if (newright == int(_messageAndReplyList.size()) - 1 && _left >= 0)
    {
        int topy = 0;
        if(_messageAndReplyList[_left].second != nullptr)
        {
           //topy = (_messageAndReplyList[_left].first->pos().y() + _messageAndReplyList[_left].second->pos().y()) / 2;
            topy    = _messageAndReplyList[_left].second->pos().y() + diff;
        }
        else
        {
            topy    = _messageAndReplyList[_left].first->pos().y() + diff;
        }
        int bottomy = topy + height();
        if (_messageAndReplyList.back().first->geometry().top() <= bottomy) haveLast = true;
        /*
        if(_messageAndReplyList[_left].second != nullptr)
        {
            //if ((_messageAndReplyList.back().first->geometry().top() +
             //       _messageAndReplyList.back().second->geometry().top()) / 2 <= bottomy) haveLast = true;
            if (_messageAndReplyList.back().second->geometry().top() <= bottomy) haveLast = true;
        }
        else
        {
            if (_messageAndReplyList.back().first->geometry().top() <= bottomy) haveLast = true;
        }
        */
    }

    if (_left >= 0 && beenResized && !haveLast)
    {
        int newy = 0;
        //int newy          = _messageAndReplyList[_left].first->pos().y();
        if(_messageAndReplyList[_left].second != nullptr)
        {
            //newy = (_messageAndReplyList[_left].first->pos().y() + _messageAndReplyList[_left].second->pos().y()) / 2;
            newy = _messageAndReplyList[_left].second->pos().y();
        }
        else
        {
            newy = _messageAndReplyList[_left].first->pos().y();
        }
        int top           = newy + diff;
        _alreadyScrolling = true;
        _scrollArea->scrollToY(top);
        _alreadyScrolling = false;
    }
    if (haveLast && beenResized)
    {
        if(_messageAndReplyList.back().second != nullptr)
        {
            //_scrollArea->scrollToY((_messageAndReplyList.back().first->pos().y() + _messageAndReplyList.back().second->pos().y()) / 2);
            _scrollArea->scrollToY(_messageAndReplyList.back().second->pos().y());
        }
        else
        {
            _scrollArea->scrollToY(_messageAndReplyList.back().first->pos().y());
        }
    }
}

void ChatHistory::resizeVisible()
{
    if (_alreadyScrolling) return;
    if (_messageAndReplyList.empty())
    {
        _left = -1;
        return;
    }
    const auto [left, right]   = findVisible();
    const std::int32_t width   = this->width() - 25;
    bool               resized = false;
    bool bHeight;
    bool bWidth;

    for (int index = left; index <= right; index++)
    {
        auto&      msg     = _messageAndReplyList[index];
        if (msg.second == nullptr)
        {
            bHeight = msg.first->expectedHeight() != msg.first->height();
            bWidth  = msg.first->width() != width;
            resized |= bHeight || bWidth;
            if (bWidth || bHeight) msg.first->resize(width, msg.first->expectedHeight());
            msg.first->setIndex(index);
        }
        else
        {
            bHeight = msg.second->expectedHeight() != msg.second->height();
            bWidth  = msg.second->width() != width;
            resized |= bHeight || bWidth;
            if (bWidth || bHeight) msg.second->resize(width, msg.second->expectedHeight());
        }
    }
    if (left >= 0 && left + 1 < int(_messageAndReplyList.size())) _left = left + 1;
    updateLayout(resized);
}

std::pair<int, int> ChatHistory::findVisible() const
{
    int left   = 0;
    int right  = int(_messageAndReplyList.size());
    int top    = _scrollArea->scrollTop();
    int bottom = top + _scrollArea->height();
    {
        int  middle = 0;
        if(_messageAndReplyList[(right - left) / 2].second != nullptr)
        {
            //middle = (_messageAndReplyList[(right - left) / 2].first->pos().y() + _messageAndReplyList[(right - left) / 2].second->pos().y()) / 2;
            middle = _messageAndReplyList[(right - left) / 2].second->pos().y();
        }
        else
        {
            middle    = _messageAndReplyList[(right - left) / 2].first->pos().y();
        }
        auto isBetween = [](int p, int t, int b) { return p >= t && p <= b; };
        while (!isBetween(middle, top, bottom) && (right - left) > 1)
        {
            if (middle < top) left = (right + left) / 2;
            if (middle > top) right = (right + left) / 2;
            //middle = _messageAndReplyList[(right + left) / 2].first->pos().y();
            if(_messageAndReplyList[(right - left) / 2].second != nullptr)
            {
                //middle = (_messageAndReplyList[(right - left) / 2].first->pos().y() + _messageAndReplyList[(right - left) / 2].second->pos().y()) / 2;
                middle = _messageAndReplyList[(right - left) / 2].second->pos().y();
            }
            else
            {
                middle = _messageAndReplyList[(right - left) / 2].first->pos().y();
            }
        }
    }
    int middle = (right + left) / 2;
    int index  = middle;
    int size   = int(_messageAndReplyList.size());

    int sizeHeight = 0;

    if(_messageAndReplyList[index].second != nullptr)
    {
        sizeHeight = _messageAndReplyList[index].second->pos().y();
    }
    else
    {
        sizeHeight = _messageAndReplyList[index].first->pos().y();
    }

    while (sizeHeight < bottom && index < size - 1)
    {
        if(_messageAndReplyList[index].second != nullptr)
        {
            //sizeHeight = (_messageAndReplyList[index].first->pos().y() + _messageAndReplyList[index].second->pos().y()) / 2;
            sizeHeight = _messageAndReplyList[index].second->pos().y();
        }
        else
        {
            sizeHeight = _messageAndReplyList[index].first->pos().y();
        }
        index++;
    }
    /*
    while ((_messageAndReplyList[index].first->pos().y() + _messageAndReplyList[index].second->pos().y()) / 2 < bottom && index < size - 1)
    {
        index++;
    }
    */
    right = index;
    index = middle;

    if(_messageAndReplyList[index].second != nullptr)
    {
        sizeHeight = _messageAndReplyList[index].second->pos().y() + _messageAndReplyList[index].second->height();
    }
    else
    {
        sizeHeight = _messageAndReplyList[index].first->pos().y() + _messageAndReplyList[index].first->height();
    }

    while(sizeHeight > top && index > 0)
    {
        if(_messageAndReplyList[index].second != nullptr)
        {
            //sizeHeight = (_messageAndReplyList[index].first->pos().y() + _messageAndReplyList[index].second->pos().y() +
             //             _messageAndReplyList[index].first->height() + _messageAndReplyList[index].second->height()) / 2;
            sizeHeight = _messageAndReplyList[index].second->pos().y() + _messageAndReplyList[index].second->height();
        }
        else
        {
            sizeHeight = _messageAndReplyList[index].first->pos().y() + _messageAndReplyList[index].first->height();
        }
        index--;
    }
    /*
    while (_messageAndReplyList[index].first->pos().y() + _messageAndReplyList[index].first->height() > top && index > 0)
    {
        index--;
    }
    */

    left = index;
    return {left, right};
}
