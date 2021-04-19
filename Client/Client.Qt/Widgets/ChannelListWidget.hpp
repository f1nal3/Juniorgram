#pragma once

#include "pch.hpp"
#include "ChannelList.hpp"
#include "FlatButton.hpp"
#include "ChannelListWindow.hpp"

class ChannelListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelListWidget(QWidget *parent = nullptr);
    ~ChannelListWidget();

public slots:
    void addChannel();
private:
   FlatButton* addChannelButton;
   ChannelListWindow* listWidgetChannels;
   QVBoxLayout* vBox;
   QVBoxLayout* vBoxFrame;
};
