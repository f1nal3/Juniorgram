#pragma once

#include "pch.hpp"

/** @class ChannelList
 *  @brief This is channel list frame
 */
class ChannelList : public QFrame
{
public:
    ChannelList(QString nameOfChannel);
protected:
    //Then it should show the chat history
    bool event(QEvent* event) override;
private: 
    std::unique_ptr<QHBoxLayout> hBoxLayout;
    std::unique_ptr<QLabel>      channelName;
};
