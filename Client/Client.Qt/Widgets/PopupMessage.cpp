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

    _animation.setTargetObject(this);
    _animation.setPropertyName("popupOpacity");
    connect(&_animation, &QAbstractAnimation::finished, this, &PopupMessage::hide);

    getPopupLabel().setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    getPopupLabel().setStyleSheet(
        "QLabel { color : white; "
        "margin-top: 6px;"
        "margin-bottom: 6px;"
        "margin-left: 10px;"
        "margin-right: 10px; }");

    int row = 0;
    int column = 0;
    _layout.addWidget(&getPopupLabel(), row, column);
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

    int offsetXY = 5;
    roundedRect.setX(rect().x() + offsetXY);
    roundedRect.setY(rect().y() + offsetXY);

    int offsetWH = 10;
    roundedRect.setWidth(rect().width() - offsetWH);
    roundedRect.setHeight(rect().height() - offsetWH);

    auto black = QColor(0, 0, 0, 180);
    painter.setBrush(QBrush(black));
    painter.setPen(Qt::NoPen);

    int xRadius = 10;
    int yRadius = 10;
    painter.drawRoundedRect(roundedRect, xRadius, yRadius);
}

void PopupMessage::popupShow()
{
    qreal transparent = 0.0;
    setWindowOpacity(transparent);

    auto visible    = popupMessageAnimation::getAnimationValue(popupMessageAnimation::Animation::VISIBLE);
    auto notVisible = popupMessageAnimation::getAnimationValue(popupMessageAnimation::Animation::NOT_VISIBLE);

    _animation.setDuration(150);
    _animation.setStartValue(notVisible);
    _animation.setEndValue(visible);

    QWidget::show();

    _animation.start();
    _timer->start(_animationDuration);
}

void PopupMessage::hideAnimation()
{
    auto visible    = popupMessageAnimation::getAnimationValue(popupMessageAnimation::Animation::VISIBLE);
    auto notVisible = popupMessageAnimation::getAnimationValue(popupMessageAnimation::Animation::NOT_VISIBLE);

    _timer->stop();
    _animation.setDuration(1000);
    _animation.setStartValue(visible);
    _animation.setEndValue(notVisible);
    _animation.start();
}

void PopupMessage::hide()
{
    int transparent = 0.0;
    if (getPopupOpacity() == transparent)
    {
        QWidget::hide();
    }
}

void PopupMessage::setShowTime(unsigned int newShowTime) 
{ 
    _animationDuration = newShowTime; }


double popupMessageAnimation::getAnimationValue(Animation animation) 
{
    if (animation == Animation::VISIBLE)
    {
        return 1.0;
    }
    else
    {
        return 0.0;
    }
}