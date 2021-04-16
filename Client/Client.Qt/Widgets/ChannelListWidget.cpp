#include "ChannelListWidget.hpp"

ChannelListWidget::ChannelListWidget(QWidget *parent) : QWidget(parent)
{
    vBox               = std::make_shared<QVBoxLayout>(this);
    vBoxFrame          = std::make_unique<QVBoxLayout>();
    listWidgetChannels = std::make_unique<ChannelListWindow>(nullptr, vBox);
    addChannelButton   = std::make_unique<FlatButton>("+");
    addChannelButton->setMinimumWidth(200);
    vBoxFrame->addWidget(addChannelButton.get());
    vBox->addLayout(vBoxFrame.get());
    vBox->addStretch();

    connect(addChannelButton.get(), &QPushButton::clicked,
           this, &ChannelListWidget::addChannel);

    setLayout(vBox.get());
}

void ChannelListWidget::addChannel()
{
    listWidgetChannels->show();
}
