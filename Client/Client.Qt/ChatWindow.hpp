#pragma once

#include <QSplitter>
#include <QStackedWidget>
#include <QWidget>
#include <memory>

#include "Widgets/ChannelListWidget.hpp"
#include "Widgets/ChatHistory.hpp"
#include "Widgets/ChatWidget.hpp"

/**
 * @class ChatWindow
 * @brief Chat form for JuniorGram
 */
class ChatWindow : public Page
{
    Q_OBJECT
public:
    /// Constructor for chat frame
    explicit ChatWindow(QWidget* parent = nullptr);

protected:
    /// Handle resize
    void resizeEvent(QResizeEvent* event) override;

private:
    std::unique_ptr<QSplitter>         _mainLayout;
    std::unique_ptr<ChannelListWidget> _channelListWidget;
    std::unique_ptr<QStackedWidget>    _chatSwitchWidget;
};
