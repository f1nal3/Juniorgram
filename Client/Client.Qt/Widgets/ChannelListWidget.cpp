#include "ChannelListWidget.hpp"

ChannelListWidget::ChannelListWidget(QWidget *parent) : QWidget(parent)
{
    vBox               = new QVBoxLayout(this);
    vBoxFrame          = new QVBoxLayout();
    listWidgetChannels = new ChannelListWindow(nullptr, vBox);
    addChannelButton   = new FlatButton("+");
    addChannelButton->setMinimumWidth(200);
    vBoxFrame->addWidget(addChannelButton);
    vBox->addLayout(vBoxFrame);
    vBox->addStretch();

    connect(addChannelButton, &QPushButton::clicked,
           this, &ChannelListWidget::addChannel);

    setLayout(vBox);
}

void ChannelListWidget::addChannel()
{
    listWidgetChannels->show();
}

ChannelListWidget::~ChannelListWidget()
{
    delete vBox;
    delete vBoxFrame;
    delete listWidgetChannels;
    delete addChannelButton;
}
