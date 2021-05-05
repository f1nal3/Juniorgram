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
    updateChannelButton   = new FlatButton("Update");
    channelList           = new ListWidget();



    vBoxLayout->addWidget(channelList);
    vBoxLayout->addWidget(addChannelButton);
    vBoxLayout->addWidget(updateChannelButton);

    connect(addChannelButton, &QPushButton::clicked, this,
            &ChannelListWindow::addChannelToMainChannelWidget);
    connect(updateChannelButton, &QPushButton::clicked, this,
            &ChannelListWindow::updateChannelListWindow);

    ConnectionManager::getClient().askForChannelList();
    updateChannelList();

    setLayout(vBoxLayout);
}

void ChannelListWindow::updateChannelList()
{
    std::thread([&]()
    {
            if(ConnectionManager::isConnected())
            {
                std::mutex mtx;
                std::unique_lock<std::mutex> lck(mtx);
                mainWidgetStatus.wait(lck);

                int numberOfCoincidences = 0;

                for(auto it = channelNames.rbegin(); it != channelNames.rend() ; ++it)
                {
                    if(channelList->count() == 0)
                    {
                        channelList->addItem(QString::fromStdString(*it));
                    }
                    for(int i = 0; i < channelList->count(); ++i)
                    {
                       if(channelList->item(i)->text() == QString::fromStdString(*it))
                       {
                           numberOfCoincidences++;
                       }
                    }
                    if(numberOfCoincidences == 0)
                    {
                        channelList->addItem(QString::fromStdString(*it));
                    }
                    numberOfCoincidences = 0;
                }

                lck.unlock();
            }
    }).detach();
}

void ChannelListWindow::addChannelToMainChannelWidget()
{
    if (channelList->currentItem())
    {
        if(channelListMainWindow->count() != 0)
        {
            int numberOfCoincidences = 0;
            for(int i = 0; i < channelListMainWindow->count(); ++i)
            {
                if(channelList->currentItem()->text() == channelListMainWindow->item(i)->text())
                {
                    numberOfCoincidences++;
                }
            }
            if(numberOfCoincidences == 0)
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

void ChannelListWindow::addChannelInfo(const std::string& nameOfChannels){ channelNames.push_back(nameOfChannels); }

void ChannelListWindow::updateChannelListWindow()
{
    if(ConnectionManager::isConnected())
    {
        ConnectionManager::getClient().askForChannelList();
        updateChannelList();
    }
}

ChannelListWindow::~ChannelListWindow()
{
    delete addChannelButton;
    delete channelList;
    delete channelListMainWindow;
    delete vBoxLayout;
}
