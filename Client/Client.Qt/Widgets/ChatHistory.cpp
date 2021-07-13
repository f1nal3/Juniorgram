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
    auto msg     = new MessageWidget(history, message, utc, user);

    history->setMinimumHeight(history->minimumHeight() + msg->expectedHeight() + 10);
    msg->show();
    msg->resize(history->width() - 25, msg->expectedHeight());

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
    auto [left, right] = findVisible();
    int  width         = this->width() - 25;
    bool resized       = false;

    for (int index = left; index <= right; index++)
    {
        auto& msg     = _messageList[index];
        bool  bHeight = msg->expectedHeight() != msg->height();
        bool  bWidth  = msg->width() != width;
        resized |= bHeight || bWidth;
        if (bWidth || bHeight) msg->resize(width, msg->expectedHeight());
        msg->setIndex(left + 1, index, right);
    }
    _left  = left + 1;
    _right = right;
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
        while (!isBetween(middle, top, bottom) && left != right)
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
