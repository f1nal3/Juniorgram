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
    history->setMinimumHeight(5000000);
    history->setMinimumHeight(0);
    _scrollArea->setOwnedWidget(std::move(history));
    _scrollArea->setWidgetResizable(true);
    connect(_scrollArea.get(), &ScrollArea::scrolled, [=]() {
        if (_messageList.empty()) return;
    });
}

void ChatHistory::resizeEvent(QResizeEvent* event)
{
    _scrollArea->resize(width(), height());
    qDebug() << _messageList.capacity() << _messageList.size();
    if (event->oldSize().width() != event->size().width())
    {
        for (auto& msg : _messageList)
        {
            msg->resize(width(), msg->height());
        }
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

    msg->resize(history->width(), msg->height());
    msg->show();

    _messageList.push_back(std::unique_ptr<MessageWidget>(msg));

    updateLayout();

    _scrollArea->scrollToWidget(msg);

    messageAdded();
}
