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
void ChatHistory::resizeVisible()
{
    if (_messageList.empty()) return;
    auto [left, right] = findVisible();
    int width          = this->width();

    for (int index = left; index <= right; index++)
    {
        auto& msg = _messageList[index];
        if (msg->width() != width) msg->resize(width, msg->height());
    }
}

void ChatHistory::resizeEvent(QResizeEvent* event)
{
    _scrollArea->resize(width(), height());
    if (event->oldSize().width() != event->size().width())
    {
        resizeVisible();
    }
    if (event->oldSize().height() != event->size().height())
    {
        updateLayout();
    }
}

void ChatHistory::updateLayout()
{
    auto bottom = _scrollArea->scrollHeight();
    for (int i = _messageList.size() - 1; i >= 0; i--)
    {
        _messageList[i]->move(0, bottom - _messageList[i]->height());
        bottom -= _messageList[i]->height() + 10;
    }
}

void ChatHistory::addMessage(const QString& message, quint64 utc, const QString& user)
{
    auto history = _scrollArea->widget();
    auto msg     = new MessageWidget(history, message, utc, user);

    history->setMinimumHeight(history->minimumHeight() + msg->height() + 10);
    // qDebug() << "ScrollTop" << _scrollArea->scrollTop() << _scrollArea->viewport()->rect();
    msg->resize(history->width(), msg->height());
    msg->show();

    _messageList.push_back(std::unique_ptr<MessageWidget>(msg));

    updateLayout();

    _scrollArea->scrollToWidget(msg);

    messageAdded();
}

std::pair<int, int> ChatHistory::findVisible() const
{
    int left   = 0;
    int right  = _messageList.size();
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
    int size   = _messageList.size();

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
