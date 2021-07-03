#include "ChatWindow.hpp"

#include <QtEvents>
#include <utility>

#include "Style/Style.hpp"
#include "Widgets/ChatHistory.hpp"

ChatWindow::ChatWindow(QWidget* parent) : QWidget(parent)
{
    setContentsMargins(0, 0, 0, 0);
    mainLayout        = std::make_unique<QSplitter>(this);
    channelListWidget = std::make_unique<ChannelListWidget>();
    chat              = std::make_unique<ChatWidget>();
    mainLayout->setOrientation(Qt::Horizontal);
    mainLayout->setChildrenCollapsible(false);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(channelListWidget.get());
    mainLayout->addWidget(chat.get());
    mainLayout->setStretchFactor(0, 20);
    mainLayout->setStretchFactor(1, 80);
}

void ChatWindow::resizeEvent(QResizeEvent*) { mainLayout->resize(width(), height()); }