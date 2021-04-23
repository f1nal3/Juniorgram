#include "ChannelListWidget.hpp"
#include "Style/Style.hpp"

ChannelListWidget::ChannelListWidget(QWidget *parent) : QWidget(parent)
{
    vBoxLayout         = new QVBoxLayout(this);
    vBoxFrame          = new QVBoxLayout();
    listWidgetChannels = new ChannelListWindow(nullptr, vBoxLayout);
    addChannelButton   = new FlatButton("+");
    vBoxFrame->addWidget(addChannelButton);
    vBoxLayout->addLayout(vBoxFrame);
    vBoxLayout->addStretch();
    connect(addChannelButton, &QPushButton::clicked,
           this, &ChannelListWidget::addChannel);

    setLayout(vBoxLayout);
}

void ChannelListWidget::addChannel()
{
    listWidgetChannels->show();
}

ChannelListWidget::~ChannelListWidget()
{
    delete vBoxLayout;
    delete vBoxFrame;
    delete listWidgetChannels;
    delete addChannelButton;
}
