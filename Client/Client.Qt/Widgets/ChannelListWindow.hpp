#pragma once

#include "pch.hpp"
#include "FlatButton.hpp"
#include "ListWidget.hpp"

/** @class ChannelListWindow
 *  @brief This is channel list
 */
class ChannelListWindow : public QWidget
{
public:
    ChannelListWindow(QWidget* parent, ListWidget* anotherChannelListWidget);
    ~ChannelListWindow();
    static void addChannelInfo(const std::string& nameOfChannels);
    inline static std::condition_variable statusMainWidget;
public slots:
    //Then have to add a channel with the chat history
    void addChannelToMainChannelWidget();
private:
    std::mutex mtx;
    inline static std::vector<std::string>* channelNames = new std::vector<std::string>();
    ListWidget*  channelList;
    FlatButton*  addChannelButton;
    QVBoxLayout* vBoxLayout;
    ListWidget*  channelListMainWindow;
};

