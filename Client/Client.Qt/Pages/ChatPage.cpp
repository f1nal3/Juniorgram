#include "ChatPage.hpp"

#include <ConnectionManager.hpp>
#include <QtEvents>
#include <iostream>
#include <thread>

ChatPage::ChatPage(QWidget* parent) : Page(parent)
{
    setContentsMargins(0, 0, 0, 0);
    _mainLayout        = std::make_unique<QSplitter>(this);
    _channelListWidget = std::make_unique<ChannelListWidget>();
    _chatSwitchWidget  = std::make_unique<QStackedWidget>();

    _mainLayout->setStyleSheet("QSplitter::handle { image: none; }");
    _mainLayout->setOrientation(Qt::Horizontal);
    _mainLayout->setChildrenCollapsible(false);
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->addWidget(_channelListWidget.get());
    _mainLayout->addWidget(_chatSwitchWidget.get());
    _mainLayout->setStretchFactor(0, 20);
    _mainLayout->setStretchFactor(1, 80);

    connect(_channelListWidget->getChannelList().get(), &ListWidget::itemPressed, this, &ChatPage::chosenChannelHandler);
}

void ChatPage::resizeEvent(QResizeEvent*) { _mainLayout->resize(width(), height()); }

void ChatPage::chosenChannelHandler()
{
    auto channelName = _channelListWidget->getChannelList()->currentItem()->text().toStdString();
    auto channel     = std::find_if
    (
        ChannelListPage::channels.begin(),
        ChannelListPage::channels.end(),
        [&channelName](const Network::ChannelInfo& channel) { return channel.channelName == channelName; }
    );
    std::cout << channel->channelName << " id: " << channel->channelID << std::endl;

    int chatIndex;
    for (chatIndex = 0; chatIndex < _chatSwitchWidget->count(); chatIndex++)
    {
        if (dynamic_cast<ChatWidget*>(_chatSwitchWidget->widget(chatIndex))->getChannelName() == channelName)
        {
            break;
        }
    }
    bool chatExists = chatIndex != _chatSwitchWidget->count();
    
    if (!chatExists)
    {
        auto chatWidget = new ChatWidget(channel->channelName);
        chatWidget->setChannelID(channel->channelID);
        _chatSwitchWidget->addWidget(chatWidget);
        connect(chatWidget, &ChatWidget::removeChannel, this, [this, chatWidget]()
        {
            _chatSwitchWidget->removeWidget(chatWidget);
            _channelListWidget->getChannelList()->clearSelection();
            delete _channelListWidget->getChannelList()->takeItem(_channelListWidget->getChannelList()->currentRow());
        });
    }
    else
    {
        auto chatWidget = dynamic_cast<ChatWidget*>(_chatSwitchWidget->widget(chatIndex));
        chatWidget->setChannelID(channel->channelID);
        chatWidget->setChannelName(channel->channelName);
    }
    _chatSwitchWidget->setCurrentIndex(chatIndex);
}