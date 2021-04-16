#pragma once

#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QEvent>
#include <QMouseEvent>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <memory>

class ChannelList : public QFrame
{
public:
    ChannelList(QString _nameChannel);
protected:
    bool event(QEvent* event) override;
private: 
    std::unique_ptr<QHBoxLayout> hbox;
    std::unique_ptr<QLabel>      nameChannel;
};
