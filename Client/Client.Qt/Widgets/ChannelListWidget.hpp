#pragma once

#include "ChannelListWindow.hpp"
#include "ConnectionManager.hpp"
#include "FlatButton.hpp"
#include "ListWidget.hpp"
#include "pch.hpp"

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
    //This slot should add chat history in the future
    void addChatHistory();
private:
    FlatButton* addChannelButton;
    ChannelListWindow* listWidgetChannels;
    QVBoxLayout* vBoxLayout;
    ListWidget* channelList;
};
