#include "ChannelListWidget.hpp"
#include "Style/Style.hpp"

ChannelListWidget::ChannelListWidget(QWidget *parent) : QWidget(parent)
{
    vBoxLayout         = new QVBoxLayout(this);
    channelList        = new ListWidget();
    listWidgetChannels = new ChannelListWindow(nullptr, channelList);
    addChannelButton   = new FlatButton("+");

    vBoxLayout->addWidget(addChannelButton);
    vBoxLayout->addWidget(channelList, 10);

    connect(addChannelButton, &QPushButton::clicked,
           this, &ChannelListWidget::addChannel);

    connect(channelList, &QListWidget::itemClicked,
            this, &ChannelListWidget::addChatHistory);

    setLayout(vBoxLayout);
}

void ChannelListWidget::addChannel()
{
    listWidgetChannels->show();
}

void ChannelListWidget::addChatHistory()
{

}

ChannelListWidget::~ChannelListWidget()
{
    //delete channelList;
    delete listWidgetChannels;
    delete addChannelButton;
    delete vBoxLayout;
}
