#include "PopupMessage.hpp"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

PopupMessage::PopupMessage(QWidget* parent) 
    : IPopupMessage(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | 
                   Qt::Tool | 
                   Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    _animation.setTargetObject(this);
    _animation.setPropertyName("popupOpacity");
    connect(&_animation, &QAbstractAnimation::finished, this, &PopupMessage::hide);

    _label.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    _label.setStyleSheet(
        "QLabel { color : white; "
        "margin-top: 6px;"
        "margin-bottom: 6px;"
        "margin-left: 10px;"
        "margin-right: 10px; }");

    _layout.addWidget(&_label, 0, 0);
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
    roundedRect.setX(rect().x() + 5);
    roundedRect.setY(rect().y() + 5);
    roundedRect.setWidth(rect().width() - 10);
    roundedRect.setHeight(rect().height() - 10);

    painter.setBrush(QBrush(QColor(0, 0, 0, 180)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(roundedRect, 10, 10);
}

void PopupMessage::setPopupText(const QString& text)
{
    _label.setText(text);
    adjustSize();
}

void PopupMessage::popupShow()
{
    setWindowOpacity(0.0);

    _animation.setDuration(150);
    _animation.setStartValue(0.0);
    _animation.setEndValue(1.0);

    QWidget::show();

    _animation.start();
    _timer->start(3000);
}

void PopupMessage::hideAnimation()
{
    _timer->stop();
    _animation.setDuration(1000);
    _animation.setStartValue(1.0);
    _animation.setEndValue(0.0);
    _animation.start();
}

void PopupMessage::hide()
{
    if (getPopupOpacity() == 0.0)
    {
        QWidget::hide();
    }
}

float PopupMessage::getPopupOpacity() const 
{ 
    return _popupOpacity; 
}

void PopupMessage::setPopupOpacity(float opacity)
{ 
    _popupOpacity = opacity;

    setWindowOpacity(opacity);
}
