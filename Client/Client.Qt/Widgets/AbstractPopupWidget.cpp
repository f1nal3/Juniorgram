#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QPainter>
#include <QStyle>

#include "Widgets/AbstractPopupWidget.hpp"

AbstractPopupWidget::AbstractPopupWidget(QWidget* parent) : QWidget(parent), _roundedRect(true), _autoHideAnimation(true)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);

    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setMouseTracking(true);

    _animation.setTargetObject(this);
    _animation.setPropertyName("popupWidgetOpacity");
    connect(&_animation, &QAbstractAnimation::finished, this, &AbstractPopupWidget::hide);

    _timer = new QTimer();
    connect(_timer, &QTimer::timeout, this, &AbstractPopupWidget::hideAnimation);
}

AbstractPopupWidget::~AbstractPopupWidget()
{
}

void AbstractPopupWidget::setPosition(QPoint position)
{
    _position = position;
}

QPoint AbstractPopupWidget::getPosition() const
{
    return _position;
}

void AbstractPopupWidget::setOpacity(float opacity)
{
    _opacity = opacity;
    setWindowOpacity(_opacity);
}

float AbstractPopupWidget::getOpacity() const
{
    return _opacity;
}

void AbstractPopupWidget::setContent()
{
    _label = new QLabel();
    _label->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    _label->setStyleSheet(
        "QLabel { color : white; "
        "margin-top: 6px;"
        "margin-bottom: 6px;"
        "margin-left: 10px;"
        "margin-right: 10px; }");
    _layout.addWidget(_label, 0, 0);
    setLayout(&_layout);

    _label->setText("some content");
    adjustSize();
}

void AbstractPopupWidget::show()
{
    if (_autoHideAnimation)
    {
        setWindowOpacity(0.0);

        _animation.setDuration(150);
        _animation.setStartValue(0.0);
        _animation.setEndValue(1.0);
    }
    else
    {
        setWindowOpacity(1.0);
    }

    setGeometry(QRect(_position, QSize(width(), height())));

    QWidget::show();

    if (_autoHideAnimation)
    {
        _animation.start();
        _timer->start(3000);
    }
}

void AbstractPopupWidget::hideAnimation()
{
    _timer->stop();
    _animation.setDuration(1000);
    _animation.setStartValue(1.0);
    _animation.setEndValue(0.0);
    _animation.start();
}

void AbstractPopupWidget::hide()
{
    if ((getPopupWidgetOpacity() == 0.0) || !_autoHideAnimation)
    {
        QWidget::hide();
    }
}

void AbstractPopupWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect paintingRect;
    paintingRect.setX(rect().x() + 5);
    paintingRect.setY(rect().y() + 5);
    paintingRect.setWidth(rect().width() - 10);
    paintingRect.setHeight(rect().height() - 10);

    painter.setBrush(Qt::black);
    painter.setPen(Qt::NoPen);

    if (_roundedRect)
    {
        painter.drawRoundedRect(paintingRect, 10, 10);
    }
    else
    {
        painter.drawRect(paintingRect);
    }
}

void AbstractPopupWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (rect().contains(event->pos()))
    {
        _timer->start();
    }
    update();
}

bool AbstractPopupWidget::getAutoHideAnimation() const
{
    return _autoHideAnimation;
}

void AbstractPopupWidget::setAutoHideAnimation(bool newAutoHideAnimation)
{
    _autoHideAnimation = newAutoHideAnimation;
    if (_autoHideAnimation)
    {
        setWindowFlag(Qt::Popup, false);
        setWindowFlag(Qt::Tool);
        setAttribute(Qt::WA_ShowWithoutActivating);
        setMouseTracking(true);
    }
    else
    {
        setWindowFlag(Qt::Tool, false);
        setWindowFlag(Qt::Popup);
        setAttribute(Qt::WA_ShowWithoutActivating, false);
        setMouseTracking(false);
    }
}

bool AbstractPopupWidget::getRoundedRect() const
{
    return _roundedRect;
}

void AbstractPopupWidget::setRoundedRect(bool newRoundedRect)
{
    _roundedRect = newRoundedRect;
}
