#include "ChannelList.hpp"

ChannelList::ChannelList()
{
    this->setMinimumSize(200, 50);
    this->setMaximumSize(200, 50);
    this->setFrameStyle(QFrame::Box);
    this->setCursor(Qt::PointingHandCursor);
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
