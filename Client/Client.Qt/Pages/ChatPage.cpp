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
        [&channelName](const Models::ChannelInfo& channel) { return channel._channelName == channelName; }
    );
    Base::Logger::FileLogger::getInstance().log
    (
        channel->_channelName + " id: " + std::to_string(channel->_channelID),
        Base::Logger::LogLevel::INFO
    );

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
        auto chatWidget = new ChatWidget(channel->_channelName);
        chatWidget->setChannelID(channel->_channelID);
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
        chatWidget->setChannelID(channel->_channelID);
        chatWidget->setChannelName(channel->_channelName);
    }
    _chatSwitchWidget->setCurrentIndex(chatIndex);
}
