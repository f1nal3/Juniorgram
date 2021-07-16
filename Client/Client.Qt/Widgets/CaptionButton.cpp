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
    setFixedWidth(Style::valueDPIScale(46));
    setFixedHeight(Style::valueDPIScale(30));

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
    QPainter p(this);

    p.setPen(Qt::NoPen);
    p.fillRect(0, 0, width(), height(), _hoverColor);

    const Style::icon& icon = _iconOverride ? *_iconOverride : _icon;

    p.drawPixmap((width() - icon->pixmap()->width()) / 2, (height() - icon->pixmap()->height()) / 2, *icon->pixmap());
    QWidget::paintEvent(event);
}
void CaptionButton::setIcon(const Style::icon* icon) { _iconOverride = icon; }
