#include "CaptionButton.hpp"

#include <QPainter>
#include <QtEvents>

#include "Style/Style.hpp"

void CaptionButton::enterEvent(QEvent* event)
{
    fadeinAnim->setDirection(QAbstractAnimation::Forward);
    fadeinAnim->start();
    AbstractButton::enterEvent(event);
}

void CaptionButton::leaveEvent(QEvent* event)
{
    fadeinAnim->setDirection(QAbstractAnimation::Backward);
    fadeinAnim->start();
    return AbstractButton::leaveEvent(event);
}

QString CaptionButton::dpi2str(int scale)
{
    if (scale <= 100)
        return "-10";
    else if (scale <= 125)
        return "-12";
    else if (scale < 200)
        return "-15";
    else if (scale < 250)
        return "-20";
    else if (scale < 300)
        return "-24";
    else
        return "-30";
}

QString CaptionButton::Lg2str(CaptionButton::CaptionLogo logo)
{
    using cp = CaptionLogo;
    if (logo == cp::Maximize)
        return "max";
    else if (logo == cp::Minimize)
        return "min";
    else if (logo == cp::Restore)
        return "restore";
    else
        return "close";
}

CaptionButton::CaptionButton(CaptionButton::CaptionLogo logo, const QColor& endColor, QWidget* parent) : AbstractButton(parent)
{
    setFixedWidth(Style::valueDPIScale(46));
    setFixedHeight(Style::valueDPIScale(30));
    int     scale = Style::getDpiScale();
    QString icon  = ":icons/" + Lg2str(logo) + "-w" + dpi2str(scale);
    pixmap        = new QPixmap(icon);

    setMouseTracking(true);
    fadeinAnim = new QPropertyAnimation(this, "hoverColor");
    fadeinAnim->setDuration(150);
    fadeinAnim->setEasingCurve(QEasingCurve::InCubic);
    fadeinAnim->setStartValue(QColor(endColor.red(), endColor.green(), endColor.blue(), 0));
    fadeinAnim->setEndValue(endColor);
    hoverColor = fadeinAnim->startValue().value<QColor>();
}

void CaptionButton::paintEvent(QPaintEvent* event)
{
    QPainter p(this);

    p.setPen(Qt::NoPen);
    p.fillRect(0, 0, width(), height(), hoverColor);

    p.drawPixmap((width() - pixmap->width()) / 2, (height() - pixmap->height()) / 2, *pixmap);
    QWidget::paintEvent(event);
}
