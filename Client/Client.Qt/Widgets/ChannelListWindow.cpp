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

    vBoxLayout          = std::make_unique<QVBoxLayout>();
    addChannelButton    = std::make_unique<FlatButton>(this, "Add");
    updateChannelButton = std::make_unique<FlatButton>(this, "Update");
    channelList         = std::make_unique<ListWidget>();

    vBoxLayout->addWidget(channelList.get());
    vBoxLayout->addWidget(addChannelButton.get());
    vBoxLayout->addWidget(updateChannelButton.get());

    addChannelButton->setClickCallback([this]() { addChannelToMainChannelWidget(); });
    updateChannelButton->setClickCallback([this]() { updateChannelListWindow(); });
    ConnectionManager::getClient().askForChannelList();
    updateChannelList();

    setLayout(vBoxLayout.get());
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

            for (int i = 0; i < channelListMainWindow->count(); ++i)
            {
                for (auto it = channelNames.rbegin(); it != channelNames.rend(); ++it)
                {
                    if (QString::fromStdString(*it) == channelListMainWindow->item(i)->text())
                    {
                        channelNames.erase(
                            std::remove(channelNames.begin(), channelNames.end(), *it),
                            channelNames.end());
                    }
                }
            }
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
                channelListMainWindow->addItem(channelList->takeItem(channelList->currentRow()));
            }
        }
        else
        {
            channelListMainWindow->addItem(channelList->takeItem(channelList->currentRow()));
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
        ConnectionManager::getClient().askForChannelList();
    }
}
