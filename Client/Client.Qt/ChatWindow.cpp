#include "ChatWindow.hpp"

#include <utility>

#include "Style/Style.hpp"

ChatWindow::ChatWindow(QWidget* parent) : QWidget(parent)
{
    setContentsMargins(0, 0, 0, 0);
    mainLayout          = new QHBoxLayout(this);
    channelListWidget   = new ChannelListWidget();
    chat                = new ChatWidget();
    
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(channelListWidget, 20);
    mainLayout->addWidget(chat, 80);
    setLayout(mainLayout);
}

ChatWindow::~ChatWindow()
{
    delete channelListWidget;
    delete rightLayout;
    delete mainLayout;
    delete chat;
}
