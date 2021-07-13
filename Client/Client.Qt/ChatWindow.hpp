#pragma once
#include <QSplitter>

#include "Widgets/ChannelListWidget.hpp"
#include "Widgets/ChatWidget.hpp"

class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWindow(QWidget* parent = nullptr);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    std::unique_ptr<QSplitter>         _mainLayout;
    std::unique_ptr<ChannelListWidget> _channelListWidget;
    std::unique_ptr<ChatWidget>        _chat;
};
