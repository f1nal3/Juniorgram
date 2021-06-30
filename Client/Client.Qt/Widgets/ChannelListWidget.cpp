#include "ChannelListWidget.hpp"

ChannelListWidget::ChannelListWidget(QWidget* parent) : QWidget(parent)
{
    vBoxLayout         = new QVBoxLayout(this);
    channelList        = new ListWidget();
    listWidgetChannels = new ChannelListWindow(channelList);
    addChannelButton   = new FlatButton(this, "+");

    setContentsMargins(0, 0, 0, 0);
    addChannelButton->setMinimumWidth(Style::valueDPIScale(200));
    vBoxLayout->addWidget(addChannelButton);
    vBoxLayout->addWidget(channelList);
    channelList->setFrameShape(QFrame::NoFrame);

    addChannelButton->setClickCallback([this]() { addChannels(); });

    vBoxLayout->setContentsMargins(contentsMargins());
    setLayout(vBoxLayout);
}

void ChannelListWidget::addChannels() { listWidgetChannels->show(); }

ChannelListWidget::~ChannelListWidget()
{
    delete addChannelButton;
    delete listWidgetChannels;
    delete vBoxLayout;
}
