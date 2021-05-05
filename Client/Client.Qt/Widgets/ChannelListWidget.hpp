#pragma once

#include "pch.hpp"
#include "ListWidget.hpp"
#include "FlatButton.hpp"
#include "ChannelListWindow.hpp"

/** @class ChannelListWidget
 *  @brief This is the main channel list widget
 */
class ChannelListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelListWidget(QWidget* parent = nullptr);
    ~ChannelListWidget();

public slots:
    void addChannels();

private:
   FlatButton*        addChannelButton;
   ChannelListWindow* listWidgetChannels;
   QVBoxLayout*       vBoxLayout;
   ListWidget*        channelList;
};
