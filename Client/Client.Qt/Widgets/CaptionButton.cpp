#include "CaptionButton.hpp"

#include <QPainter>
#include <QtEvents>

void CaptionButton::enterEvent(QEvent* event)
{
    _fadeinAnim->setDirection(QAbstractAnimation::Forward);
    _fadeinAnim->start();
    return AbstractButton::enterEvent(event);
}

void CaptionButton::leaveEvent(QEvent* event)
{
    _fadeinAnim->setDirection(QAbstractAnimation::Backward);
    _fadeinAnim->start();
    return AbstractButton::leaveEvent(event);
}

CaptionButton::CaptionButton(QWidget* parent, const QColor& endColor, const Style::icon& icon) : AbstractButton(parent), _icon(icon)
{
    resize(Style::valueDPIScale(46) / Style::devicePixelRatio(), Style::valueDPIScale(30) / Style::devicePixelRatio());

    setMouseTracking(true);
    _fadeinAnim = new QPropertyAnimation(this, "_hoverColor");
    _fadeinAnim->setDuration(150);
    _fadeinAnim->setEasingCurve(QEasingCurve::InCubic);
    _fadeinAnim->setStartValue(QColor(endColor.red(), endColor.green(), endColor.blue(), 0));
    _fadeinAnim->setEndValue(endColor);
    _hoverColor = _fadeinAnim->startValue().value<QColor>();
}

void CaptionButton::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    painter.fillRect(0, 0, width(), height(), _hoverColor);

    const int          maxSide = Style::valueDPIScale(20);
    const Style::icon& icon    = _iconOverride ? *_iconOverride : _icon;

    const int horside = (width() - maxSide) / 2;
    const int verside = (height() - maxSide) / 2;

    if (icon.size().width() <= maxSide)
    {
        painter.drawPixmap((width() - icon->pixmap()->width()) / 2, (height() - icon->pixmap()->height()) / 2, *icon->pixmap());
    }
    else
    {
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.drawPixmap(QRect(horside, verside, maxSide, maxSide), *icon->pixmap());
    }
    QWidget::paintEvent(event);
}

bool CaptionButton::setIcon(const Style::icon* icon)
{
    if (icon != nullptr)
    {
        if (icon->size().width() != icon->size().height()) return false;
    }
    _iconOverride = icon;
    return true;
}
