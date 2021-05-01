#include "ChannelListWindow.hpp"
#include "ConnectionManager.hpp"

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

    connect(addChannelButton, &QPushButton::clicked, this,
            &ChannelListWindow::addChannelToMainChannelWidget);


    if (ConnectionManager::isConnected())
    {
        ConnectionManager::getClient().askForChannelList();

        /*
         *  without this doesn't work, because thread where work ConnectionManager
         *  add channel names immediately while main thread is working
         *  so he doesn't have time to add channels to the list here
        */ 
        // std::this_thread::sleep_for(std::chrono::milliseconds(150));

       /*
        * Another one way
       */
        std::unique_lock<std::mutex> lck(mtx);
        statusMainWidget.wait(lck);

        for(std::size_t i = 0; i < channelNames->size(); ++i)
        {
           channelList->addItem(QString::fromStdString(channelNames->at(i)));
        }
    }

    setLayout(vBoxLayout);
}

void ChannelListWindow::addChannelToMainChannelWidget()
{
    if (channelList->currentItem())
    {
        if(channelListMainWindow->count() != 0)
        {
            int hitCount = 0;
            for(int i = 0; i < channelListMainWindow->count(); ++i)
            {
                if(channelList->currentItem()->text() == channelListMainWindow->item(i)->text())
                {
                    hitCount++;
                }
            }
            if(hitCount == 0)
            {
                channelListMainWindow->addItem(channelList->currentItem()->text());
            }
        }
        else
        {
            channelListMainWindow->addItem(channelList->currentItem()->text());
        }
    }
    this->hide();
}

void ChannelListWindow::addChannelInfo(const std::string& nameOfChannels){ channelNames->push_back(nameOfChannels); }

ChannelListWindow::~ChannelListWindow()
{
    delete addChannelButton;
    delete channelList;
    delete channelListMainWindow;
    delete vBoxLayout;
}
