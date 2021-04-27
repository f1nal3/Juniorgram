#include "ChannelListWindow.hpp"
#include "Style/Style.hpp"

ChannelListWindow::ChannelListWindow(QWidget* parent, ListWidget* anotherChannelList)
    : QWidget(parent), channelListMainWindow(anotherChannelList)
{
    setStyleSheet(
        "QWidget {"
        "background-color: #323232;"
        "}");
    setFixedWidth(Style::maxDPI);
    setFixedHeight(Style::maxDPI);
    vBoxLayout            = new QVBoxLayout;
    addChannelButton      = new FlatButton("Add");
    channelList           = new ListWidget();

    vBoxLayout->addWidget(channelList);
    vBoxLayout->addWidget(addChannelButton);

    connect(addChannelButton, &QPushButton::clicked,
                     this, &ChannelListWindow::addChannelToMainChannelWidget);

    setLayout(vBoxLayout);
}

void ChannelListWindow::addChannelToMainChannelWidget()
{
    if(channelList->currentItem())
    {
        channelListMainWindow->addItem(channelList->currentItem()->text());
    }
    this->hide();
}

ChannelListWindow::~ChannelListWindow()
{
    delete addChannelButton;
    //delete channelList;
    delete channelListMainWindow;
    delete vBoxLayout;
}
