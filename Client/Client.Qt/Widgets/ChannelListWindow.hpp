#pragma once

#include <Network/Primitives.hpp>
#include <QVBoxLayout>
#include <QWidget>

#include "ListWidget.hpp"
#include "Page.hpp"

class FlatButton;

/**
 *  @class ChannelListWindow
 *  @brief This is channel list
 */
class ChannelListWindow : public Page
{
public:
    /// Constructor for channel list window
    explicit ChannelListWindow(std::shared_ptr<ListWidget>& anotherChannelListWidget, QWidget* parent = nullptr);

public:
    /// TODO: rework channel list in main window to make this private
    inline static std::vector<Network::ChannelInfo> channels;

private slots:

    /// set channels
    void setChannels(const std::vector<Network::ChannelInfo>& channels_);

    /// Adds selected channel to main window
    void addChannelToMainChannelWidget();

    /// Do a request to server for new channels
    void requestChannels();
    /// Update channel view
    void updateChannelList();

private:
    inline static std::map<std::uint64_t, bool> _channelsAddMap;
    std::unique_ptr<ListWidget>                 _channelList;
    std::unique_ptr<FlatButton>                 _addChannelButton;
    std::unique_ptr<FlatButton>                 _updateChannelButton;
    std::unique_ptr<QVBoxLayout>                _vBoxLayout;
    std::shared_ptr<ListWidget>                 _widgetChannelList;
};
