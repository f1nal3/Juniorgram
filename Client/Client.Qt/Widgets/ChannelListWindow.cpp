#include "ChannelListWindow.hpp"

#include "Style/Style.hpp"

ChannelListWindow::ChannelListWindow(QWidget* parent, QVBoxLayout* vBoxAnotherLayout)
    : QWidget(parent), vBoxChannelListWidget(vBoxAnotherLayout)
{
    setFixedWidth(Style::maxDPI);
    setFixedHeight(Style::maxDPI);
    vBoxLayout       = new QVBoxLayout;
    addChannelButton = new FlatButton("Add");
    channelList      = new QListWidget(this);
    vBoxLayout->addWidget(channelList);
    vBoxLayout->addWidget(addChannelButton);

    connect(addChannelButton, &QPushButton::clicked, this,
            &ChannelListWindow::addChannelToMainChannelWidget);

    setLayout(vBoxLayout);
}

void ChannelListWindow::addChannelToMainChannelWidget()
{
    if (channelList->currentItem())
    {
        ChannelList* newChannel = new ChannelList(channelList->currentItem()->text());
        vBoxChannelListWidget->addWidget(newChannel);
    }
    this->hide();
}

ChannelListWindow::~ChannelListWindow()
{
    delete vBoxLayout;
    delete addChannelButton;
    delete channelList;
    delete vBoxChannelListWidget;
}
