#pragma once

#include "pch.hpp"
#include "ChannelList.hpp"
#include "FlatButton.hpp"

/** @class ChannelListWindow
 *  @brief This is channel list
 */
class ChannelListWindow : public QWidget
{
public:
    ChannelListWindow(QWidget* parent, QVBoxLayout* vbox);
    ~ChannelListWindow();
public slots:
//Then have to add a channel with the chat history
    void addChannelToMainChannelWidget();
private:
    QListWidget* channelList;
    FlatButton*  addChannelButton;
    QVBoxLayout* vBox;
    QVBoxLayout* vBoxChannelListWidget;
};
