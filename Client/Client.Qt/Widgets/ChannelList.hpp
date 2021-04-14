#pragma once

#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QEvent>
#include <QMouseEvent>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <memory>

class ChannelList : public QFrame
{
public:
    ChannelList();
protected:
    bool event(QEvent* event) override;
private:
};
