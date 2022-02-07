#include "PopupSystemMessage.hpp"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

PopupSystemMessage::PopupSystemMessage(QWidget* parent) 
	: QWidget(parent) 
{
    setWindowFlags(Qt::FramelessWindowHint |    
                   Qt::Tool |                    
                   Qt::WindowStaysOnTopHint);    
    setAttribute(Qt::WA_TranslucentBackground);  
    setAttribute(Qt::WA_ShowWithoutActivating);  

    animation.setTargetObject(this);            
    animation.setPropertyName("popupOpacity");  
    connect(&animation, &QAbstractAnimation::finished, this, &PopupSystemMessage::hide);

    label.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label.setStyleSheet(
        "QLabel { color : white; "
        "margin-top: 6px;"
        "margin-bottom: 6px;"
        "margin-left: 10px;"
        "margin-right: 10px; }");

    layout.addWidget(&label, 0, 0);
    setLayout(&layout);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &PopupSystemMessage::hideAnimation);
}

void PopupSystemMessage::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)


    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect roundedRect;
    roundedRect.setX(rect().x() + 5);
    roundedRect.setY(rect().y() + 5);
    roundedRect.setWidth(rect().width() - 10);
    roundedRect.setHeight(rect().height() - 10);

    painter.setBrush(QBrush(QColor(0, 0, 0, 180)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(roundedRect, 10, 10);
}

void PopupSystemMessage::setPopupText(const QString& text)
{ 
    label.setText(text);
    adjustSize();
}

void PopupSystemMessage::show()
{
    setWindowOpacity(0.0);

    animation.setDuration(150);
    animation.setStartValue(0.0);
    animation.setEndValue(1.0);

    /*
    setGeometry(QApplication::desktop()->availableGeometry().width() - 36 - width() + QApplication::desktop()->availableGeometry().x(),
                QApplication::desktop()->availableGeometry().height() - 36 - height() + QApplication::desktop()->availableGeometry().y(),
                width(), height());  
                                                                                                                                                moving the widget to the bottom corner
                */

     
    QWidget::show();

    animation.start();
    timer->start(3000);
}

void PopupSystemMessage::hideAnimation()
{
    timer->stop();              
    animation.setDuration(1000); 
    animation.setStartValue(1.0);  
    animation.setEndValue(0.0); 
    animation.start();
}

void PopupSystemMessage::hide()
{
    if (getPopupOpacity() == 0.0)
    {
        QWidget::hide();
    }
}

void PopupSystemMessage::setPopupOpacity(float opacity)
{ 
    popupOpacity = opacity;

    setWindowOpacity(opacity);
}

float PopupSystemMessage::getPopupOpacity() const 
{ 
    return popupOpacity; 
}