#include "ChannelList.hpp"
#include "Style/Style.hpp"

ChannelList::ChannelList(QString _nameChannel)
{
    hbox        = std::make_unique<QHBoxLayout>();
    nameChannel = std::make_unique<QLabel>(_nameChannel);
    hbox->addWidget(nameChannel.get());
    setFixedWidth(Style::valueDPIScale(200));
    setFixedHeight(Style::valueDPIScale(60));
    this->setFrameStyle(QFrame::Box);
    this->setCursor(Qt::PointingHandCursor);
    setLayout(hbox.get());
}

//Then it should show the chat history
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
