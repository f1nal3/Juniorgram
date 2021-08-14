#include "ChannelListWindow.hpp"

#include "Application.hpp"

ChannelListWindow::ChannelListWindow(std::shared_ptr<ListWidget>& anotherChannelListWidget, QWidget* parent)
    : QWidget(parent), _widgetChannelList(anotherChannelListWidget)
{
    setStyleSheet(
        QString("QWidget {"
                "background-color: #323232;"
                "}"));
    setWindowFlag(Qt::WindowMinimizeButtonHint, false);
    setWindowFlag(Qt::WindowStaysOnTopHint);
    setWindowModality(Qt::ApplicationModal);

    setFixedWidth(Style::valueDPIScale(300));
    setFixedHeight(Style::valueDPIScale(250));

    _vBoxLayout          = std::make_unique<QVBoxLayout>();
    _addChannelButton    = std::make_unique<FlatButton>(this, "Add");
    _updateChannelButton = std::make_unique<FlatButton>(this, "Update");
    _channelList         = std::make_unique<ListWidget>();

    _vBoxLayout->addWidget(_channelList.get());
    _vBoxLayout->addWidget(_addChannelButton.get());
    _vBoxLayout->addWidget(_updateChannelButton.get());

    connect(ReceiverManager::instance(), &ReceiverManager::onChannelListRequest, this, &ChannelListWindow::setChannels);

    _addChannelButton->setClickCallback([this]() { addChannelToMainChannelWidget(); });
    _updateChannelButton->setClickCallback([this]() { requestChannels(); });
    requestChannels();

    setLayout(_vBoxLayout.get());
}

void ChannelListWindow::updateChannelList()
{
    for (const auto& channel : channels)
    {
        if (!_channelsAddMap[channel.channelID])
        {
            _channelList->addItem(QString::fromStdString(channel.channelName));
            _channelsAddMap[channel.channelID] = true;
        }
    }
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

void ChannelListWindow::setChannels(const std::vector<Network::ChannelInfo>& newChannels)
{
    channels = newChannels;
    for (auto& channel : channels)
    {
        _channelsAddMap.emplace(std::pair<std::uint64_t, bool>(channel.channelID, false));
    }
    updateChannelList();
}

void ChannelListWindow::requestChannels()
{
    if (oApp->connectionManager()->isConnected())
    {
        oApp->connectionManager()->askForChannelList();
    }
}
