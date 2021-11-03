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

    connect(_channelListWidget->getChannelList().get(), &ListWidget::itemPressed, [&]() {
        auto findChannelID = [&]() {
            const auto channelName = _channelListWidget->getChannelList()->currentItem()->text().toStdString();
            auto       channelIterator =
                std::find_if(ChannelListPage::channels.begin(), ChannelListPage::channels.end(),
                             [&](const Network::ChannelInfo& channelInfo) { return channelName == channelInfo.channelName; });

            std::cout << channelIterator->channelName << " id: " << channelIterator->channelID << std::endl;

            return channelIterator->channelID;
        };

        if (_chatSwitchWidget->widget(_channelListWidget->getChannelList()->currentRow()) == nullptr)
        {
            auto channelID = findChannelID();
            auto channelName =
                std::find_if(ChannelListPage::channels.begin(), ChannelListPage::channels.end(), [channelID](Network::ChannelInfo a) {
                    return a.channelID == channelID;
                })->channelName;
            auto chatWidget = new ChatWidget(channelName);
            chatWidget->setChannelID(channelID);
            _chatSwitchWidget->insertWidget(_channelListWidget->getChannelList()->currentRow(), chatWidget);
            connect(chatWidget, &ChatWidget::removeChannel, this, [this]() {
                _chatSwitchWidget->removeWidget(_chatSwitchWidget->widget(_channelListWidget->getChannelList()->currentRow()));
                _channelListWidget->getChannelList()->takeItem(_channelListWidget->getChannelList()->currentRow());
                _channelListWidget->getChannelList()->clearSelection();
            });
        }
        else
        {
            auto channelID  = findChannelID();
            auto chatWidget = dynamic_cast<ChatWidget*>(_chatSwitchWidget->widget(_channelListWidget->getChannelList()->currentRow()));
            chatWidget->setChannelID(channelID);
            auto channelName =
                std::find_if(ChannelListPage::channels.begin(), ChannelListPage::channels.end(), [channelID](Network::ChannelInfo a) {
                    return a.channelID == channelID;
                })->channelName;
            chatWidget->setChannelName(channelName);
        }
        _chatSwitchWidget->setCurrentIndex(_channelListWidget->getChannelList()->currentRow());
    });
}

void ChatPage::resizeEvent(QResizeEvent*) { _mainLayout->resize(width(), height()); }
