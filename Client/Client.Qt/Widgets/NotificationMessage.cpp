#include "NotificationMessage.hpp"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

NotificationMessage::NotificationMessage(QWidget* parent) 
    : AbstractNotificationMessage(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | 
                   Qt::Tool | 
                   Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    setWindowOpacity(_transparent);

    _animation.setTargetObject(this);
    _animation.setPropertyName("_notificationOpacity");
    connect(&_animation, &QAbstractAnimation::finished, this, &NotificationMessage::hide);

    getNotificationLabel().setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    getNotificationLabel().setStyleSheet(
        "QLabel { color : white; "
        "margin-top: 6px;"
        "margin-bottom: 6px;"
        "margin-left: 10px;"
        "margin-right: 10px; }");

    _layout.addWidget(&getNotificationLabel(), _row_widgetPosition, _column_widgetPosition);
    setLayout(&_layout);

    _timer = std::make_unique<QTimer>();
    connect(_timer.get(), &QTimer::timeout, this, &NotificationMessage::hideAnimation);
}

void NotificationMessage::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect roundedRect;

    roundedRect.setX(rect().x() + _offsetXY);
    roundedRect.setY(rect().y() + _offsetXY);

    roundedRect.setWidth(rect().width() - _offsetWH);
    roundedRect.setHeight(rect().height() - _offsetWH);

    painter.setBrush(QBrush(_black));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(roundedRect, _xRadius, _yRadius);
}

void NotificationMessage::notificationShow()
{

    _animation.setDuration(_animationValue.animationTimeShow);
    _animation.setStartValue(_animationValue.not_transparent);
    _animation.setEndValue(_animationValue.transparent);

    QWidget::show();

    _animation.start();
    _timer->start(_animationDuration);
}

void NotificationMessage::hideAnimation()
{ 
    _timer->stop();
    _animation.setDuration(_animationValue.animationTimeHide);
    _animation.setStartValue(_animationValue.transparent);
    _animation.setEndValue(_animationValue.not_transparent);
    _animation.start();
}

void NotificationMessage::hide()
{
    if (getNotificationOpacity() == _animationValue.not_transparent)
    {
        QWidget::hide();
    }
}

void NotificationMessage::setAnimationDuration(uint32_t newAnimationDuration) { 
    _animationDuration = newAnimationDuration; 
}
