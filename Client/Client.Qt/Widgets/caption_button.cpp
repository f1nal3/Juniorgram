#include "caption_button.h"
#include "../Style/style.h"

void CaptionButton::enterEvent(QEvent *event) {
    Q_UNUSED(event);
    fadein_anim->start();
}

void CaptionButton::leaveEvent(QEvent *event) {
    Q_UNUSED(event);
    fadein_anim->stop();
    close_button = fadein_anim->startValue().value<QColor>();

    update();
}

void CaptionButton::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit mouseRelease();
    }
}

QString CaptionButton::dpi2str(int scale) {
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

QString CaptionButton::Lg2str(CaptionButton::CaptionLogo logo) {
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

CaptionButton::CaptionButton(CaptionButton::CaptionLogo logo, const QColor &end_color,QWidget *parent)
        : QWidget(parent) {
    setFixedWidth(Style::WindowsScaleDPIValue(46));
    setFixedHeight(Style::WindowsScaleDPIValue(30));
    int scale = Style::getDpiScale();
    QString icon = ":icons/" + Lg2str(logo) + "-w" + dpi2str(scale);
    pixmap = new QPixmap(icon);

    setMouseTracking(true);
    fadein_anim = new QPropertyAnimation(this, "close_button");
    fadein_anim->setDuration(150);
    fadein_anim->setEasingCurve(QEasingCurve::InCubic);
    fadein_anim->setStartValue(QColor(end_color.red(), end_color.green(), end_color.blue(), 0));
    fadein_anim->setEndValue(end_color);
    close_button = fadein_anim->startValue().value<QColor>();
}

void CaptionButton::paintEvent(QPaintEvent *event) {
    QPainter p(this);

    p.setPen(Qt::NoPen);
    p.fillRect(0, 0, width(), height(), close_button);

    p.drawPixmap((width() - pixmap->width()) / 2, (height() - pixmap->height()) / 2, *pixmap);
    QWidget::paintEvent(event);
}
