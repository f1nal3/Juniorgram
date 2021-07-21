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

void ChatHistory::addMessage(const QString& message, quint64 utc, const QString& user)
{
    auto history = _scrollArea->widget();
    //The _messageId is incremented for message numbering
    auto msg     = new MessageWidget(history, message, _messageId++, _userId, utc, user);

    msg->show();
    msg->resize(history->width() - 25, msg->expectedHeight());
    history->setMinimumHeight(history->minimumHeight() + msg->expectedHeight() + 10);

    _messageList.push_back(std::unique_ptr<MessageWidget>(msg));

    connect(msg, &MessageWidget::geometryChanged, this, [=](int diff) {
        history->setMinimumHeight(history->minimumHeight() + diff);
        updateLayout(true);
    });

    updateLayout();

    _scrollArea->scrollToWidget(msg);

    messageAdded();
}

void ChatHistory::clear() { _messageList.clear(); }

void ChatHistory::deleteMessage(int userId, int messageId)
{
    for (typename std::vector<std::unique_ptr<MessageWidget>>::iterator i = _messageList.begin(); i != _messageList.end(); i++)
    {
        if ((i->get()->_messageId == messageId) && (i->get()->_userId == userId))
            i->get()->onDelete();
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
        y = _messageList[_left]->pos().y();
        diff -= y;
    }
    for (int i = int(_messageList.size()) - 1; i >= 0; i--)
    {
        _messageList[i]->move(0, bottom - _messageList[i]->height());
        bottom -= _messageList[i]->height() + 10;
    }

    bool haveLast = false;

    auto [newleft, newright] = findVisible();

    if (newright == int(_messageList.size()) - 1 && _left >= 0)
    {
        int topy    = _messageList[_left]->pos().y() + diff;
        int bottomy = topy + height();
        if (_messageList.back()->geometry().top() <= bottomy) haveLast = true;
    }

    if (_left >= 0 && beenResized && !haveLast)
    {
        int newy          = _messageList[_left]->pos().y();
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

    for (int index = left; index <= right; index++)
    {
        auto&      msg     = _messageList[index];
        const bool bHeight = msg->expectedHeight() != msg->height();
        const bool bWidth  = msg->width() != width;
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
        int  middle    = _messageList[(right - left) / 2]->pos().y();
        auto isBetween = [](int p, int t, int b) { return p >= t && p <= b; };
        while (!isBetween(middle, top, bottom) && (right - left) > 1)
        {
            if (middle < top) left = (right + left) / 2;
            if (middle > top) right = (right + left) / 2;
            middle = _messageList[(right + left) / 2]->pos().y();
        }
    }
    int middle = (right + left) / 2;
    int index  = middle;
    int size   = int(_messageList.size());

    while (_messageList[index]->pos().y() < bottom && index < size - 1)
    {
        index++;
    }
    right = index;
    index = middle;
    while (_messageList[index]->pos().y() + _messageList[index]->height() > top && index > 0)
    {
        index--;
    }
    left = index;
    return {left, right};
}
