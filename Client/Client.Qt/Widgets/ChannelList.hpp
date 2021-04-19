#pragma once

#include "pch.hpp"

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
