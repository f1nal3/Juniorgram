#pragma once

#include <QSplitter>
#include <QStackedWidget>
#include <QWidget>
#include <memory>
#include <algorithm>

#include "Widgets/ChannelListWidget.hpp"
#include "Widgets/ChatHistory.hpp"
#include "Widgets/ChatWidget.hpp"

/**
 * @class ChatPage
 * @brief Chat page for JuniorGram
 */
class ChatPage : public Page
{
    Q_OBJECT
public:
    /// Constructor for chat page
    explicit ChatPage(QWidget* parent = nullptr);

protected:
    /// Handle resize
    void resizeEvent(QResizeEvent* event) override;

private:
    std::unique_ptr<QSplitter>         _mainLayout;
    std::unique_ptr<ChannelListWidget> _channelListWidget;
    std::unique_ptr<QStackedWidget>    _chatSwitchWidget;
};
