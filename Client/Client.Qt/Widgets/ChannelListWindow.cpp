#include "ChannelListWindow.hpp"

#include <QFutureWatcher>

#include "ConnectionManager.hpp"
#include "Style/Style.hpp"

ChannelListWindow::ChannelListWindow(ListWidget* anotherChannelListWidget, QWidget* parent)
    : QWidget(parent), channelListMainWindow(anotherChannelListWidget)
{
    setStyleSheet(
        QString("QWidget {"
                "background-color: #323232;"
                "}"));
    setWindowFlag(Qt::WindowMinimizeButtonHint, false);

    setFixedWidth(Style::valueDPIScale(300));
    setFixedHeight(Style::valueDPIScale(250));

    vBoxLayout          = new QVBoxLayout;
    addChannelButton    = new FlatButton(this, "Add");
    updateChannelButton = new FlatButton(this, "Update");
    channelList         = new ListWidget();

    vBoxLayout->addWidget(channelList);
    vBoxLayout->addWidget(addChannelButton);
    vBoxLayout->addWidget(updateChannelButton);

    addChannelButton->setClickCallback([this]() { addChannelToMainChannelWidget(); });
    updateChannelButton->setClickCallback([this]() { updateChannelListWindow(); });
    ConnectionManager::getClient().askForChannelList();
    updateChannelList();

    setLayout(vBoxLayout);
}

void ChannelListWindow::updateChannelList()
{
    std::thread([&]() {
        if (ConnectionManager::isConnected())
        {
            std::mutex                   mtx;
            std::unique_lock<std::mutex> lck(mtx);
            mainWidgetStatus.wait(lck);

            int numberOfCoincidences = 0;

            for (auto it = channelNames.rbegin(); it != channelNames.rend(); ++it)
            {
                if (channelList->count() == 0)
                {
                    channelList->addItem(QString::fromStdString(*it));
                }
                for (int i = 0; i < channelList->count(); ++i)
                {
                    if (channelList->item(i)->text() == QString::fromStdString(*it))
                    {
                        numberOfCoincidences++;
                    }
                }
                if (numberOfCoincidences == 0)
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
        if (channelListMainWindow->count() != 0)
        {
            int numberOfCoincidences = 0;
            for (int i = 0; i < channelListMainWindow->count(); ++i)
            {
                if (channelList->currentItem()->text() == channelListMainWindow->item(i)->text())
                {
                    numberOfCoincidences++;
                }
            }
            if (numberOfCoincidences == 0)
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

void ChannelListWindow::addChannelInfo(const std::string& nameOfChannels)
{
    channelNames.push_back(nameOfChannels);
}

void ChannelListWindow::updateChannelListWindow()
{
    if (ConnectionManager::isConnected())
    {
        static int                   i = 0;
        static QFutureInterface<int> fi;
        i++;
        if (i > 5)
        {
            fi.reportFinished();
        }
        auto future = fi.future();

        ConnectionManager::getClient().askForChannelList();
        auto* watcher = new QFutureWatcher<int>();

        connect(watcher, &QFutureWatcher<int>::finished, [=]() { std::cout << i; });

        watcher->setFuture(future);
    }
}

ChannelListWindow::~ChannelListWindow()
{
    delete addChannelButton;
    delete channelListMainWindow;
    delete vBoxLayout;
}
