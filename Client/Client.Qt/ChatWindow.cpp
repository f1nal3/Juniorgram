#include "ChatWindow.hpp"

#include <QtEvents>
#include <iostream>
#include <ConnectionManager.hpp>
#include <thread>

ChatWindow::ChatWindow(QWidget* parent) : QWidget(parent)
{
    setContentsMargins(0, 0, 0, 0);
    _mainLayout        = std::make_unique<QSplitter>(this);
    _channelListWidget = std::make_unique<ChannelListWidget>();
    _chatSwitchWidget  = std::make_unique<QStackedWidget>();
    _mainLayout->setOrientation(Qt::Horizontal);
    _mainLayout->setChildrenCollapsible(false);
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->addWidget(_channelListWidget.get());
    _mainLayout->addWidget(_chatSwitchWidget.get());
    _mainLayout->setStretchFactor(0, 20);
    _mainLayout->setStretchFactor(1, 80);

    connect(_channelListWidget->getChannelList().get(), &ListWidget::itemPressed, [&]()
    {
        auto findChannelID = [&]() 
        {
            const auto channelName = _channelListWidget->getChannelList()->currentItem()->text().toStdString();
            auto channelItartor    = std::find_if(ChannelListWindow::channels.begin(), 
                                                  ChannelListWindow::channels.end(),
                                                  [&](const Network::ChannelInfo& channelInfo)
                                                  {
                                                      return channelName == channelInfo.channelName;
                                                  });

            std::cout << channelItartor->channelName << " " << channelItartor->channelID << std::endl;
            
            return channelItartor->channelID;
        };

        if (_chatSwitchWidget->currentIndex() < 0)
        {
            auto chatWidget = new ChatWidget();
            chatWidget->setChannelID(findChannelID());
            _chatSwitchWidget->insertWidget(_channelListWidget->getChannelList()->currentRow(), chatWidget);
        }

        if(_chatSwitchWidget->widget(_channelListWidget->getChannelList()->currentRow()) == nullptr)
        {
            auto chatWidget = new ChatWidget();
            chatWidget->setChannelID(findChannelID());
            _chatSwitchWidget->insertWidget(_channelListWidget->getChannelList()->currentRow(), chatWidget);
        }
        _chatSwitchWidget->setCurrentIndex(_channelListWidget->getChannelList()->currentRow());
    });
}

void ChatWindow::resizeEvent(QResizeEvent*) { _mainLayout->resize(width(), height()); }
