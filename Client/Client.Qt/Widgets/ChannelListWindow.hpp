#pragma once

#include <QVBoxLayout>
#include <QWidget>
#include <condition_variable>
#include <Network/Primitives.hpp>

#include "Buttons.hpp"
#include "ListWidget.hpp"
#include "MainWidget.hpp"

/** @class ChannelListWindow
 *  @brief This is channel list
 */
class ChannelListWindow : public QWidget
{
public:
    ChannelListWindow(std::shared_ptr<ListWidget>& anotherChannelListWidget, QWidget* parent = nullptr);
    /**
     * @brief Method for update channel list window in another thread.
     */
    void updateChannelList();
    /**
     * @brief Static method for adding name of channels in own non-static ListWidget container.
     * @param Name of Channels as std::string
     */
    static void setChannels(std::vector<Network::ChannelInfo>&& channels_);

public:
    inline static std::condition_variable           mainWidgetStatus;
    inline static std::vector<Network::ChannelInfo> channels;

public slots:
    /**
     * @brief method for adding name of the selected channel in main channel window.
     */
    void addChannelToMainChannelWidget();
    /**
     * @brief method for adding new name of channels in list channels window.
     */
    void updateChannelListWindow();

private:
    inline static std::map<std::uint64_t, bool> _channelsAddMap;
    std::unique_ptr<ListWidget>                 _channelList;
    std::unique_ptr<FlatButton>                 _addChannelButton;
    std::unique_ptr<FlatButton>                 _updateChannelButton;
    std::unique_ptr<QVBoxLayout>                _vBoxLayout;
    std::shared_ptr<ListWidget>                 _widgetChannelList;
};
