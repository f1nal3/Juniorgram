#pragma once

#include <Network/Primitives.hpp>
#include <QWidget>

#include "Buttons.hpp"
#include "Application.hpp"

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
               QString  channelName);
    void setChannelName(const QString channelName) { _channelName = channelName; }

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

private slots:
    void leaveChannel();
    void deleteChannel();

private:
    void updateLayout();

private:
    std::unique_ptr<FlatButton> _leaveChannel;
    std::unique_ptr<FlatButton> _deleteChannel;
    QString _channelName;
};
