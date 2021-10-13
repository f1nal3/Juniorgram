#pragma once

#include <Network/Primitives.hpp>
#include <QWidget>

#include "Buttons.hpp"

/**
 * @class ChannelBar
 * @brief Bar which shows info about channel on top
 * @todo Styles, somehow connect it to top level
 */
class ChannelBar : public QWidget
{
    Q_OBJECT
public:
    /// Default constructor
    ChannelBar(QWidget* parent,
               QString  channelName = "haha" /*, const Network::ChannelInfo& info*/);  // for the future, remove channelName to info

protected:
    /// Handle paint
    void paintEvent(QPaintEvent* event) override;
    /// Handle resize
    void resizeEvent(QResizeEvent* event) override;

signals:
    /// On leave button click
    void leaveChannelClick();
    /// On delete button click
    void deleteChannelClick();

private:
    void updateLayout();

private:
    std::unique_ptr<FlatButton> _leaveChannel;
    std::unique_ptr<FlatButton> _deleteChannel;

    const Network::ChannelInfo _info;

    QString _channelName;
};
