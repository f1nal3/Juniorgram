#include "PopupMessage.hpp"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

PopupMessage::PopupMessage(QWidget* parent) 
    : AbstractPopupMessage(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | 
                   Qt::Tool | 
                   Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    setWindowOpacity(_transparent);

    _animation.setTargetObject(this);
    _animation.setPropertyName("_popupOpacity");
    connect(&_animation, &QAbstractAnimation::finished, this, &PopupMessage::hide);

    getPopupLabel().setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    getPopupLabel().setStyleSheet(
        "QLabel { color : white; "
        "margin-top: 6px;"
        "margin-bottom: 6px;"
        "margin-left: 10px;"
        "margin-right: 10px; }");

    _layout.addWidget(&getPopupLabel(), _row_widgetPosition, _column_widgetPosition);
    setLayout(&_layout);

    _timer = std::make_unique<QTimer>();
    connect(_timer.get(), &QTimer::timeout, this, &PopupMessage::hideAnimation);
}

void PopupMessage::paintEvent(QPaintEvent* event)
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

void PopupMessage::popupShow()
{

    _animation.setDuration(_animationValue.animationTimeShow);
    _animation.setStartValue(_animationValue.not_transparent);
    _animation.setEndValue(_animationValue.transparent);

    QWidget::show();

    _animation.start();
    _timer->start(_animationDuration);
}

void PopupMessage::hideAnimation()
{ 
    _timer->stop();
    _animation.setDuration(_animationValue.animationTimeHide);
    _animation.setStartValue(_animationValue.transparent);
    _animation.setEndValue(_animationValue.not_transparent);
    _animation.start();
}

void PopupMessage::hide()
{
    if (getPopupOpacity() == _animationValue.not_transparent)
    {
        QWidget::hide();
    }
}

void PopupMessage::setAnimationDuration(uint32_t newAnimationDuration) { 
    _animationDuration = newAnimationDuration; 
}
