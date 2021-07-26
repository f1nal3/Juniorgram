#include "ChatWindow.hpp"

#include <QtEvents>


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

    connect(_channelListWidget->getChannelList().get(), &ListWidget::itemPressed,
            [&](){

        int numberOfCoincidences = 0;
        if(_chatSwitchWidget->currentIndex() < 0)
        {
                _chatSwitchWidget->insertWidget(_channelListWidget->getChannelList()->currentRow(), new ChatWidget());
                _channelListNumbers.push_back(_channelListWidget->getChannelList()->currentRow());
        }
        for(size_t i = 0; i < _channelListNumbers.size(); ++i)
        {
            if(_channelListNumbers[i] == _channelListWidget->getChannelList()->currentRow())
            {
                _chatSwitchWidget->setCurrentIndex(_channelListWidget->getChannelList()->currentRow());
                numberOfCoincidences++;
            }
        }
        if(numberOfCoincidences == 0)
        {
            _chatSwitchWidget->insertWidget(_channelListWidget->getChannelList()->currentRow(), new ChatWidget());
            _chatSwitchWidget->setCurrentIndex(_channelListWidget->getChannelList()->currentRow());
            _channelListNumbers.push_back(_channelListWidget->getChannelList()->currentRow());
        }
        numberOfCoincidences = 0;

    });
}

void ChatWindow::resizeEvent(QResizeEvent*) { _mainLayout->resize(width(), height()); }
