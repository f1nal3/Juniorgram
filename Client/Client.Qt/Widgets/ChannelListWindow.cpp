#include "ChannelListWindow.hpp"
#include "Style/Style.hpp"

ChannelListWindow::ChannelListWindow(QWidget* parent, QVBoxLayout* vbox)
    : QWidget(parent), vBoxChannelListWidget(vbox)
{
    setFixedWidth(Style::maxDPI);
    setFixedHeight(Style::maxDPI);
    vBox                  = new QVBoxLayout;
    addChannelButton      = new FlatButton("Add");
    channelList           = new QListWidget(this);
    vBox->addWidget(channelList);
    vBox->addWidget(addChannelButton);

    connect(addChannelButton, &QPushButton::clicked,
                     this, &ChannelListWindow::addChannelToMainChannelWidget);

    setLayout(vBox);
}

void ChannelListWindow::addChannelToMainChannelWidget()
{
    if(channelList->currentItem())
    {
        ChannelList* newChannel = new ChannelList(channelList->currentItem()->text());
        vBoxChannelListWidget->addWidget(newChannel);
    }
    this->hide();
}

ChannelListWindow::~ChannelListWindow()
{
    delete vBox;
    delete addChannelButton;
    delete channelList;
    delete vBoxChannelListWidget;
}
