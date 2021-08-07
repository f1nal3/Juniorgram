#include "ChannelListWindow.hpp"

#include <QFutureWatcher>

#include "ConnectionManager.hpp"

ChannelListWindow::ChannelListWindow(std::shared_ptr<ListWidget>& anotherChannelListWidget, QWidget* parent)
    : QWidget(parent), _widgetChannelList(anotherChannelListWidget)
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
    updateChannelListWindow();

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

            for (auto channel : channels)
            {
                if (!_channelsAddMap[channel.channelID])
                {
                    _channelList->addItem(QString::fromStdString(channel.channelName));
                    _channelsAddMap[channel.channelID] = true;
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
        if (_widgetChannelList->count() != 0)
        {
            int numberOfCoincidences = 0;
            for (int i = 0; i < _widgetChannelList->count(); ++i)
            {
                if (_channelList->currentItem()->text() == _widgetChannelList->item(i)->text())
                {
                    numberOfCoincidences++;
                }
            }
            if (numberOfCoincidences == 0)
            {
                _widgetChannelList->addItem(_channelList->takeItem(_channelList->currentRow()));
            }
        }
        else
        {
            _widgetChannelList->addItem(_channelList->takeItem(_channelList->currentRow()));
        }
    }
}

void ChannelListWindow::setChannels(std::vector<Network::ChannelInfo>&& channels_)
{ 
    channels = channels_;
    for (auto& channel : channels)
    {
        _channelsAddMap.emplace(std::pair<std::uint64_t, bool>(channel.channelID, false));
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
