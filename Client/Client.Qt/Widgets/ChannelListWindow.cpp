#include "ChannelListWindow.hpp"

ChannelListWindow::ChannelListWindow(QWidget* parent, std::shared_ptr<QVBoxLayout> _vbox) : QWidget(parent)
{
    this->resize(500, 500);
    vBoxChannelListWidget = _vbox;
    vBox                  = std::unique_ptr<QVBoxLayout>();
    addChannelButton      = std::make_unique<FlatButton>("Add");
    channelList           = std::make_unique<QListWidget>(this);
    vBox->addWidget(channelList.get());
    vBox->addWidget(addChannelButton.get());

    connect(addChannelButton.get(), &QPushButton::clicked,
                     this, &ChannelListWindow::addChannelToMainChannelWidget);

    setLayout(vBox.get());
}

//Then have to add a channel with the chat history
void ChannelListWindow::addChannelToMainChannelWidget()
{
    ChannelList* newChannel = new ChannelList(channelList->currentItem()->text());
    vBoxChannelListWidget->addWidget(newChannel);
    this->hide();
}
