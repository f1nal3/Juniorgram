#include "LogoWidget.hpp"

LogoWidget::LogoWidget(QWidget* parent) : QWidget(parent)
{
    logo = new QPixmap(":images/textlogo.png");
}
LogoWidget::~LogoWidget() { delete logo; }
void LogoWidget::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::RenderHint::LosslessImageRendering);
    p.setRenderHint(QPainter::RenderHint::SmoothPixmapTransform);
    p.setRenderHint(QPainter::RenderHint::Antialiasing);
    p.drawPixmap(0, 0, width(), height(), *logo);
    QWidget::paintEvent(event);
}
