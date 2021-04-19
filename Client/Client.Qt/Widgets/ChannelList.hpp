#pragma once

#include "pch.hpp"

/** @class ChannelList
 *  @brief This is channel list frame
 */
class ChannelList : public QFrame
{
public:
    ChannelList(QString _channelName);
protected:
    //Then it should show the chat history
    bool event(QEvent* event) override;
private: 
    std::unique_ptr<QHBoxLayout> hbox;
    std::unique_ptr<QLabel>      channelName;
};
