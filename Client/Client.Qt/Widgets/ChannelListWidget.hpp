#pragma once

#include <QVBoxLayout>
#include <QWidget>
#include <QShortcut>

#include "ListWidget.hpp"
#include "Pages/ChannelCreationPage.hpp"
#include "Pages/ChannelListPage.hpp"

class FlatButton;

/**
 *  @class ChannelListWidget
 *  @brief This is the main channel list widget
 */
class ChannelListWidget : public QWidget
{
    Q_OBJECT
public:
    /// Constructor for channel list widget
    explicit ChannelListWidget(QWidget* parent = nullptr);

public slots:
    /// Shows channel list window
    void showChannelListWindow();
    /// Shows channel list window
    void showChannelCreationWindow();

public:
    /// Returns channel list
    std::shared_ptr<ListWidget> getChannelList();

private:
    std::unique_ptr<ChannelListPage>     _listWidgetChannels;
    std::unique_ptr<ChannelCreationPage> _channelCreationPopup;

    std::unique_ptr<FlatButton>  _addChannelButton;
    std::unique_ptr<FlatButton>  _createChannelButton;
    std::unique_ptr<QVBoxLayout> _mainLayout;
    std::unique_ptr<QHBoxLayout> _buttonLayout;
    std::shared_ptr<ListWidget>  _channelList;

    std::unique_ptr<QShortcut>   _openChannelList;
};
