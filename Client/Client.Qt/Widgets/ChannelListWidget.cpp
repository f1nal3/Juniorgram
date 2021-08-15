#include "ChannelListWidget.hpp"

ChannelListWidget::ChannelListWidget(QWidget* parent) : QWidget(parent)
{
    _vBoxLayout       = std::make_unique<QVBoxLayout>(this);
    _channelList      = std::make_shared<ListWidget>();
    _addChannelButton = std::make_unique<FlatButton>(this, "+");

    setContentsMargins(0, 0, 0, 0);
    setMinimumWidth(Style::valueDPIScale(200));
    setMaximumWidth(Style::valueDPIScale(300));
    _vBoxLayout->addWidget(_addChannelButton.get());
    _vBoxLayout->addWidget(_channelList.get());
    _channelList->setFrameShape(QFrame::NoFrame);

    _addChannelButton->setClickCallback([this]() { showChannelListWindow(); });

    _vBoxLayout->setContentsMargins(contentsMargins());
    setLayout(_vBoxLayout.get());
}

void ChannelListWidget::showChannelListWindow()
{
    if (!_listWidgetChannels) _listWidgetChannels = std::make_unique<ChannelListWindow>(_channelList);
    _listWidgetChannels->show();
}

std::shared_ptr<ListWidget> ChannelListWidget::getChannelList() { return _channelList; }
