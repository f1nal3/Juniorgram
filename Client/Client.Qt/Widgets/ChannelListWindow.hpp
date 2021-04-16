#pragma once
#include "ChannelList.hpp"
#include <QWidget>
#include "FlatButton.hpp"
#include <QListWidget>
#include <QVBoxLayout>


class ChannelListWindow : public QWidget
{
public:
    ChannelListWindow(QWidget* parent, std::shared_ptr<QVBoxLayout> _vbox);
public slots:
    void addChannelToMainChannelWidget();
private:
    std::unique_ptr<QListWidget> channelList;
    std::unique_ptr<FlatButton>  addChannelButton;
    std::unique_ptr<QVBoxLayout> vBox;
    std::shared_ptr<QVBoxLayout> vBoxChannelListWidget;
};
