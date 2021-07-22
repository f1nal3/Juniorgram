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

CaptionButton::CaptionButton(QWidget* parent, const Style::TitleBarButton* st) : AbstractButton(parent), _st(st)
{
    setMouseTracking(true);
    _fadeinAnim = std::make_unique<QPropertyAnimation>(this, "_hoverColor");
    updateWidget();
}

void CaptionButton::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    painter.fillRect(0, 0, width(), height(), _hoverColor);

    const int          maxSide = Style::valueDPIScale(20);
    const Style::icon& icon    = _iconOverride ? *_iconOverride : _st->icon;

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

void CaptionButton::setStyle(const Style::TitleBarButton* newSt)
{
    _st = newSt;
    updateWidget();
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
void CaptionButton::updateWidget()
{
    resize(_st->width, _st->height);

    _fadeinAnim->setDuration(150);
    _fadeinAnim->setEasingCurve(QEasingCurve::InCubic);
    _fadeinAnim->setStartValue(_st->bgColor->color);
    _fadeinAnim->setEndValue(_st->overBgColor->color);
    _hoverColor = _fadeinAnim->startValue().value<QColor>();
    update();
}
