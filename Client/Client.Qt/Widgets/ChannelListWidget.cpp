#include "ChannelListWidget.hpp"

#include "Style/Style.hpp"

ChannelListWidget::ChannelListWidget(QWidget* parent) : QWidget(parent)
{
    vBoxLayout         = new QVBoxLayout(this);
    channelList        = new ListWidget();
    listWidgetChannels = new ChannelListWindow(nullptr, channelList);
    addChannelButton   = new FlatButton("+");

    addChannelButton->setMinimumWidth(Style::valueDPIScale(200));
    vBoxLayout->addWidget(addChannelButton);
    vBoxLayout->addWidget(channelList, 10);

    connect(addChannelButton, &QPushButton::clicked, this, &ChannelListWidget::addChannels);

    connect(channelList, &QListWidget::itemClicked, this, &ChannelListWidget::addChatHistory);

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

void ChannelListWidget::addChatHistory() {}

ChannelListWidget::~ChannelListWidget()
{
    // delete channelList;
    delete listWidgetChannels;
    delete addChannelButton;
    delete vBoxLayout;
}
