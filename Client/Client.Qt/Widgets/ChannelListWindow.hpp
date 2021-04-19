#pragma once

#include "pch.hpp"
#include "ChannelList.hpp"
#include "FlatButton.hpp"

class ChannelListWindow : public QWidget
{
public:
    ChannelListWindow(QWidget* parent, QVBoxLayout* _vbox);
    ~ChannelListWindow();
public slots:
    void addChannelToMainChannelWidget();
private:
    QListWidget* channelList;
    FlatButton*  addChannelButton;
    QVBoxLayout* vBox;
    QVBoxLayout* vBoxChannelListWidget;
};
