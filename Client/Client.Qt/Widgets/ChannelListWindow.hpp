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
    ChannelListWindow(ListWidget* anotherChannelListWidget, QWidget* parent = nullptr);
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
    std::unique_ptr<ListWidget> channelList;
    std::unique_ptr<FlatButton> addChannelButton;
    std::unique_ptr<FlatButton> updateChannelButton;
    std::unique_ptr<QVBoxLayout> vBoxLayout;
    std::unique_ptr<ListWidget> channelListMainWindow;
};
