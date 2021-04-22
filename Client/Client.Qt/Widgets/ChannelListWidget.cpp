#include "ChannelListWidget.hpp"

ChannelListWidget::ChannelListWidget(QWidget *parent) : QWidget(parent)
{
    vBoxLayout         = new QVBoxLayout(this);
    vBoxFrame          = new QVBoxLayout();
    listWidgetChannels = new ChannelListWindow(nullptr, vBoxLayout);
    addChannelButton   = new FlatButton("+");
    addChannelButton->setMinimumWidth(200);
    vBoxFrame->addWidget(addChannelButton);
    vBoxLayout->addLayout(vBoxFrame);
    vBoxLayout->addStretch();

    connect(addChannelButton, &QPushButton::clicked,
           this, &ChannelListWidget::addChannels);

    setLayout(vBoxLayout);
}

void ChannelListWidget::addChannels() {
    if (ConnectionManager::isConnected())
    {
        ConnectionManager::getClient().askForChannelList();
    }
    listWidgetChannels->show();
}

ChannelListWidget::~ChannelListWidget()
{
    delete vBoxLayout;
    delete vBoxFrame;
    delete listWidgetChannels;
    delete addChannelButton;
}
