#include "ChannelListPage.hpp"

#include "Application.hpp"
#include "Widgets/Buttons.hpp"

ChannelListPage::ChannelListPage(std::shared_ptr<ListWidget>& anotherChannelListWidget, QWidget* parent)
    : Page(parent), _widgetChannelList(anotherChannelListWidget)
{
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

    connect(ReceiverManager::instance(), &ReceiverManager::onChannelListRequest, this, &ChannelListPage::setChannels);
    connect(ReceiverManager::instance(), &ReceiverManager::onChannelSubscribingListAnswer, this, &ChannelListPage::addSubscribedChannelToMainChannelWidget);

    _addChannelButton->setClickCallback([this]() { addChannelToChannelListWidget(); });
    _updateChannelButton->setClickCallback([this]() { requestChannels(); });
    requestChannels();

    setLayout(_vBoxLayout.get());
}

void ChannelListPage::updateChannelList()
{
    for (const auto& channel : channels)
    {
        if (!_channelsAddMap[channel.channelID])
        {
            _channelList->addItem(QString::fromStdString(channel.channelName));
            _channelsAddMap[channel.channelID] = true;
        }
    }
    onResume();
}

void ChannelListPage::addChannelToChannelListWidget()
{
    if (_channelList->currentItem())
    {
        std::string channel = _channelList->currentItem()->text().toStdString();
        auto        channelInfo =
            std::find_if(channels.begin(), channels.end(), [channel](const Network::ChannelInfo& i) 
            { return i.channelName == channel; });
        if (oApp->connectionManager()->isConnected())
        {
            oApp->connectionManager()->subscriptionChannel(channelInfo->channelID);
        }
        _widgetChannelList->addItem(_channelList->takeItem(_channelList->currentRow()));
    }
}

void ChannelListPage::setChannels(const std::vector<Network::ChannelInfo>& newChannels)
{
    channels = newChannels;
    for (auto& channel : channels)
    {
        _channelsAddMap.emplace(std::pair<std::uint64_t, bool>(channel.channelID, false));
    }
    updateChannelList();
}

void ChannelListPage::addSubscribedChannelToMainChannelWidget(const std::vector<uint64_t>& ChannelSubscribeList)
{
    std::vector<std::string> channelsSubscribeVector;
    for (auto channel : ChannelSubscribeList)
    {
        int  row = 0;
        auto findChannel =
            std::find_if(channels.begin(), channels.end(), [channel](Network::ChannelInfo i) 
                                                           { return i.channelID == channel; });
        channelsSubscribeVector.push_back(findChannel->channelName);
        _channelList->setCurrentRow(row);
        while (_channelList->currentRow() != -1)
        {
            if (_channelList->item(_channelList->currentRow())->text().toStdString() == findChannel->channelName)
            {
                _widgetChannelList->addItem(_channelList->takeItem((_channelList->currentRow())));
                break;
            }
            _channelList->setCurrentRow(++row);
        }
    }
}

void ChannelListPage::requestChannels()
{
    onPause();
    if (oApp->connectionManager()->isConnected())
    {
        oApp->connectionManager()->askForChannelList();
        oApp->connectionManager()->askForSubscriptionChannelList();
    }
}
