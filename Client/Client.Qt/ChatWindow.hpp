#pragma once

#include <QSplitter>
#include <QWidget>
#include <memory>

#include "Widgets/ChannelListWidget.hpp"
#include "Widgets/ChatHistory.hpp"
#include "Widgets/ChatWidget.hpp"

/**
 * @class ChatWindow
 * @brief Chat for JuniorGram
 */
class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for chat form
     * @param parent MainWidget
     */
    explicit ChatWindow(QWidget* parent = nullptr);

protected:
    /**
     * @brief Handle resize events
     * @param event Resize event
     */
    void resizeEvent(QResizeEvent* event) override;

private:
    std::unique_ptr<QSplitter>         _mainLayout;
    std::unique_ptr<ChannelListWidget> _channelListWidget;
    std::unique_ptr<ChatWidget>        _chat;
};
