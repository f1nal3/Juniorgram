#include "LogoWidget.hpp"

#include <QPainter>

#include "Style/StyleBasic.hpp"

LogoWidget::LogoWidget(QWidget* parent) : QWidget(parent) { _logo = std::make_unique<QPixmap>(":images/textlogo.png"); }

void LogoWidget::setPart(double percent) { _part = percent; }

void LogoWidget::paintEvent(QPaintEvent* paintEvent)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(0, 0, width(), height(), *_logo);
    QWidget::paintEvent(paintEvent);
}
void LogoWidget::updateSize()
{
    if (auto parent = parentWidget())
    {
        const auto         FIT_MAX     = bestFit();
        const auto minTopShift = static_cast<std::int32_t>(parent->height() * _part / 100);
        const double       aspectRatio = FIT_MAX.width() / FIT_MAX.height();

        const int  SPACE      = Style::valueDPIScale(10);
        const auto FIT_WIDTH  = QSize(parent->width() - SPACE * 2, static_cast<int>((parent->width() - SPACE * 2) / aspectRatio));
        const auto FIT_HEIGHT = QSize(static_cast<int>((minTopShift - SPACE * 2) * aspectRatio), minTopShift - SPACE * 2);

        auto bestFit = FIT_MAX;
        if (FIT_WIDTH.width() < bestFit.width()) bestFit = FIT_WIDTH;
        if (FIT_HEIGHT.width() < bestFit.width()) bestFit = FIT_HEIGHT;

        resize(bestFit);
        move((parent->width() - bestFit.width()) / 2, (minTopShift - bestFit.height()) / 2);
    }
}
