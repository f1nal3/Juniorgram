#include "ChannelListWidget.hpp"

ChannelListWidget::ChannelListWidget(QWidget* parent) : QWidget(parent)
{
    vBoxLayout         = std::make_unique<QVBoxLayout>(this);
    channelList        = new ListWidget();
    listWidgetChannels = new ChannelListWindow(channelList);
    addChannelButton   = std::make_unique<FlatButton>(this, "+");

    setContentsMargins(0, 0, 0, 0);
    addChannelButton->setMinimumWidth(Style::valueDPIScale(200));
    vBoxLayout->addWidget(addChannelButton.get());
    vBoxLayout->addWidget(channelList);
    channelList->setFrameShape(QFrame::NoFrame);

    addChannelButton->setClickCallback([this]() { addChannels(); });

    vBoxLayout->setContentsMargins(contentsMargins());
    setLayout(vBoxLayout.get());
}

void ChannelListWidget::addChannels() { listWidgetChannels->show(); }
