#pragma once

#include <QVBoxLayout>
#include <QWidget>
#include <condition_variable>

#include "FlatButton.hpp"
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
     * @param Name of Channels as std::map<std::string, uint16_t>
     */
    static void addChannelInfoMap(const std::map<std::string, uint16_t>& nameOfChannelsMap);

public:
    inline static std::condition_variable mainWidgetStatus;

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
    struct ChannelStruct
    {
        uint16_t channelId;
        bool     addToMainChannelsWindow = false;
    };
    inline static std::map<std::string, ChannelStruct> _channelsMap{};
    std::unique_ptr<ListWidget>                        _channelList;
    std::unique_ptr<FlatButton>                        _addChannelButton;
    std::unique_ptr<FlatButton>                        _updateChannelButton;
    std::unique_ptr<QVBoxLayout>                       _vBoxLayout;
    std::shared_ptr<ListWidget>                        _channelListMainWindow;
};
