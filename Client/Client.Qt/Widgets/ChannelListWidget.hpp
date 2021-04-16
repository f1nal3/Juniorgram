#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include "ChannelList.hpp"
#include "FlatButton.hpp"
#include "ChannelListWindow.hpp"
#include <QListWidget>
#include <memory>

class ChannelListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelListWidget(QWidget *parent = nullptr);

public slots:
    void addChannel();
private:
   std::unique_ptr<FlatButton> addChannelButton;
   std::unique_ptr<ChannelListWindow> listWidgetChannels;
   std::shared_ptr<QVBoxLayout> vBox;
   std::unique_ptr<QVBoxLayout> vBoxFrame;
};
