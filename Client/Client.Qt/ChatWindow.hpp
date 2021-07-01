#pragma once
#include "Widgets/ChannelListWidget.hpp"
#include "Widgets/ChatWidget.hpp"

class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWindow(QWidget* parent = nullptr);
    ~ChatWindow() override;

private:
    QHBoxLayout* mainLayout;
    ChannelListWidget* channelListWidget;
    ChatWidget* chat;
};
