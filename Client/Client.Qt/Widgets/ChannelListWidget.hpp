#pragma once

#include "pch.hpp"
#include "ChannelList.hpp"
#include "FlatButton.hpp"
#include "ChannelListWindow.hpp"
#include "ConnectionManager.hpp"

/** @class ChannelListWidget
 *  @brief This is the main channel list widget
 */
class ChannelListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelListWidget(QWidget *parent = nullptr);
    ~ChannelListWidget();

public slots:
    void addChannels();

private:
   FlatButton* addChannelButton;
   ChannelListWindow* listWidgetChannels;
   QVBoxLayout* vBoxLayout;
   QVBoxLayout* vBoxFrame;
};
