#pragma once

#include <QWidget>
#include <QVBoxLayout>

#include "ChannelListWindow.hpp"
#include "FlatButton.hpp"
#include "ListWidget.hpp"
/**
 *  @class ChannelListWidget
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
    std::unique_ptr<FlatButton> addChannelButton;
    std::unique_ptr<QVBoxLayout> vBoxLayout;
    ChannelListWindow* listWidgetChannels;
    ListWidget* channelList;
};
