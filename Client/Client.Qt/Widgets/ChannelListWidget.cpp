#include "ChannelListWidget.hpp"

ChannelListWidget::ChannelListWidget(QWidget *parent) : QWidget(parent)
{
    vBox               = std::make_shared<QVBoxLayout>(this);
    listWidgetChannels = std::make_unique<ChannelListWindow>(nullptr, vBox);
    addChannelButton   = std::make_unique<FlatButton>("+");
    addChannelButton->setMinimumWidth(200);
    vBox->addWidget(addChannelButton.get());

    connect(addChannelButton.get(), &QPushButton::clicked,
           this, &ChannelListWidget::addChannel);

    setLayout(vBox.get());
}

void ChannelListWidget::addChannel()
{
    listWidgetChannels->show();
}
