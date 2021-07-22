#pragma once

#include <QVBoxLayout>
#include <QWidget>

#include "Buttons.hpp"
#include "ChannelListWindow.hpp"
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

public slots:
    void addChannels();

private:
    std::unique_ptr<FlatButton>        _addChannelButton;
    std::unique_ptr<ChannelListWindow> _listWidgetChannels;
    std::unique_ptr<QVBoxLayout>       _vBoxLayout;
    std::shared_ptr<ListWidget>        _channelList;
};
