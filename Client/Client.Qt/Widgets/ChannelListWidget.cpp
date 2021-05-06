#include "ChannelListWidget.hpp"

#include "Style/Style.hpp"

ChannelListWidget::ChannelListWidget(QWidget* parent) : QWidget(parent)
{
    vBoxLayout         = new QVBoxLayout(this);
    channelList        = new ListWidget();
    listWidgetChannels = new ChannelListWindow(channelList);
    addChannelButton   = new FlatButton("+");

    addChannelButton->setMinimumWidth(Style::valueDPIScale(200));
    vBoxLayout->addWidget(addChannelButton);
    vBoxLayout->addWidget(channelList, 10);

    connect(addChannelButton, &QPushButton::clicked, this, &ChannelListWidget::addChannels);

    setLayout(vBoxLayout);
}

void ChannelListWidget::addChannels() { listWidgetChannels->show(); }

ChannelListWidget::~ChannelListWidget()
{
    delete listWidgetChannels;
    delete addChannelButton;
    delete vBoxLayout;
}
