#pragma once

#include "FlatButton.hpp"
#include "ListWidget.hpp"
#include "MainWidget.hpp"
#include "pch.hpp"

/** @class ChannelListWindow
 *  @brief This is channel list
 */
class ChannelListWindow : public QWidget
{
public:
    ChannelListWindow(ListWidget* anotherChannelListWidget, QWidget* parent = nullptr);
    ~ChannelListWindow();
    /**
     * @brief Method for update channel list window in another thread.
     */
    void updateChannelList();
    /**
     * @brief Static method for adding name of channels in own non-static ListWidget container.
     * @param Name of Channels as std::string
     */
    static void addChannelInfo(const std::string& nameOfChannels);

public:
    inline static std::condition_variable mainWidgetStatus;

public slots:
    void addChannelToMainChannelWidget();
    void updateChannelListWindow();

private:
    inline static std::vector<std::string> channelNames{};
    ListWidget*                            channelList;
    FlatButton*                            addChannelButton;
    FlatButton*                            updateChannelButton;
    QVBoxLayout*                           vBoxLayout;
    ListWidget*                            channelListMainWindow;
};
