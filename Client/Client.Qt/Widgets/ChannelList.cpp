#include "ChannelList.hpp"
#include "Style/Style.hpp"

ChannelList::ChannelList(QString nameOfChannel)
{
    hBoxLayout        = std::make_unique<QHBoxLayout>();
    channelName = std::make_unique<QLabel>(nameOfChannel);
    hBoxLayout->addWidget(channelName.get());
    setFixedWidth(Style::valueDPIScale(200));
    setFixedHeight(Style::valueDPIScale(60));
    this->setFrameStyle(QFrame::Box);
    this->setCursor(Qt::PointingHandCursor);
    setLayout(hBoxLayout.get());
}

bool ChannelList::event(QEvent* event)
{
   if(event->type() == QEvent::MouseButtonPress)
   {
       QMouseEvent* m_event = static_cast<QMouseEvent*>(event);
       if(m_event->button() == Qt::LeftButton)
       {
           return true;
       }
   }
   return QFrame::event(event);
}
