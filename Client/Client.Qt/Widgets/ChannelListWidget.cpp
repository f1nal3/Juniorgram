#include "ChannelListWidget.hpp"

#include "Style/Style.hpp"

ChannelListWidget::ChannelListWidget(QWidget* parent) : QWidget(parent)
{
    vBoxLayout         = new QVBoxLayout(this);
    channelList        = new ListWidget();
    listWidgetChannels = new ChannelListWindow(nullptr, channelList);
    addChannelButton   = new FlatButton("+");
    vBoxLayout->addWidget(addChannelButton);
    vBoxLayout->addWidget(channelList, 10);

    connect(addChannelButton, &QPushButton::clicked,
           this, &ChannelListWidget::addChannel);

    setLayout(vBoxLayout);
}

void ChannelListWidget::addChannels() {

    if (ConnectionManager::isConnected())
    {
        ConnectionManager::getClient().askForChannelList();

        //here should be code of adding channelList to our listWidgetChannels
    }
    listWidgetChannels->show();
}

ChannelListWidget::~ChannelListWidget()
{
    delete channelList;
    delete listWidgetChannels;
    delete addChannelButton;
    delete vBoxLayout;
}
