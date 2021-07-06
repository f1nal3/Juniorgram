#include "LogoWidget.hpp"
#include <QPainter>

LogoWidget::LogoWidget(QWidget* parent) : QWidget(parent)
{
    logo = std::make_unique<QPixmap>(":images/textlogo.png");
}

void LogoWidget::paintEvent(QPaintEvent* paintEvent)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(0, 0, width(), height(), *logo);
    QWidget::paintEvent(paintEvent);
}
