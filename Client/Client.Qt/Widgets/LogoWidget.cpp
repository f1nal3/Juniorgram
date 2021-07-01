#include "LogoWidget.hpp"
#include <QPainter>

LogoWidget::LogoWidget(QWidget* parent) : QWidget(parent)
{
    logo = new QPixmap(":images/textlogo.png");
}

LogoWidget::~LogoWidget() { delete logo; }

void LogoWidget::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawPixmap(0, 0, width(), height(), *logo);
    QWidget::paintEvent(event);
}
