#include "ChannelListWindow.hpp"

#include <QFutureWatcher>

#include "ConnectionManager.hpp"
#include "Style/Style.hpp"

ChannelListWindow::ChannelListWindow(std::shared_ptr<ListWidget>& anotherChannelListWidget, QWidget* parent)
    : QWidget(parent), _channelListMainWindow(anotherChannelListWidget)
{
    setStyleSheet(
        QString("QWidget {"
                "background-color: #323232;"
                "}"));
    setWindowFlag(Qt::WindowMinimizeButtonHint, false);

    setFixedWidth(Style::valueDPIScale(300));
    setFixedHeight(Style::valueDPIScale(250));

    _vBoxLayout          = std::make_unique<QVBoxLayout>();
    _addChannelButton    = std::make_unique<FlatButton>(this, "Add");
    _updateChannelButton = std::make_unique<FlatButton>(this, "Update");
    _channelList         = std::make_unique<ListWidget>();

    _vBoxLayout->addWidget(_channelList.get());
    _vBoxLayout->addWidget(_addChannelButton.get());
    _vBoxLayout->addWidget(_updateChannelButton.get());

    _addChannelButton->setClickCallback([this]() { addChannelToMainChannelWidget(); });
    _updateChannelButton->setClickCallback([this]() { updateChannelListWindow(); });
    
    if (ConnectionManager::isConnected())
        ConnectionManager::getClient().askForChannelList();
    updateChannelList();

    setLayout(_vBoxLayout.get());
}

void ChannelListWindow::updateChannelList()
{
    std::thread([&]() {
        if (ConnectionManager::isConnected())
        {
            std::mutex                   mtx;
            std::unique_lock<std::mutex> lck(mtx);
            mainWidgetStatus.wait(lck);
            for (auto &channel : _channelsMap)
            {
                if (!channel.second.addToMainChannelsWindow)
                {
                    _channelList->addItem(QString::fromStdString(channel.first));
                    channel.second.addToMainChannelsWindow = true;
                }
            }
            lck.unlock();
        }
    }).detach();
}

void ChannelListWindow::addChannelToMainChannelWidget()
{
    if (_channelList->currentItem())
    {
        if (_channelListMainWindow->count() != 0)
        {
            int numberOfCoincidences = 0;
            for (int i = 0; i < _channelListMainWindow->count(); ++i)
            {
                if (_channelList->currentItem()->text() == _channelListMainWindow->item(i)->text())
                {
                    numberOfCoincidences++;
                }
            }
            if (numberOfCoincidences == 0)
            {
                _channelListMainWindow->addItem(_channelList->takeItem(_channelList->currentRow()));
            }
        }
        else
        {
            _channelListMainWindow->addItem(_channelList->takeItem(_channelList->currentRow()));
        }
    }
}

void ChannelListWindow::addChannelInfoMap(const std::map<std::string, uint16_t> &nameOfChannelsMap)
{
    for (auto channel : nameOfChannelsMap)
    {
        ChannelStruct channelInfo;
        channelInfo.channelId = channel.second;
        _channelsMap.insert(std::pair(channel.first, channelInfo));
    }
}

void ChannelListWindow::updateChannelListWindow()
{
    if (ConnectionManager::isConnected())
    {
        ConnectionManager::getClient().askForChannelList();
        updateChannelList();
    }
}
