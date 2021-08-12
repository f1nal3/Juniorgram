#include "LogoWidget.hpp"

#include <QPainter>

#include "Style/StyleBasic.hpp"

LogoWidget::LogoWidget(QWidget* parent) : QWidget(parent), _partInPercent(40) { _logo = std::make_unique<QPixmap>(":images/textlogo.png"); }

void LogoWidget::setPart(int percent) { _partInPercent = percent; }

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
        const std::int32_t minTopShift = parent->height() * _partInPercent / 100;
        const float        aspectRatio = FIT_MAX.width() / FIT_MAX.height();

        const int  SPACE      = Style::valueDPIScale(10);
        const auto FIT_WIDTH  = QSize(parent->width() - SPACE * 2, (parent->width() - SPACE * 2) / aspectRatio);
        const auto FIT_HEIGHT = QSize((minTopShift - SPACE * 2) * aspectRatio, minTopShift - SPACE * 2);

        auto bestFit = FIT_MAX;
        if (FIT_WIDTH.width() < bestFit.width()) bestFit = FIT_WIDTH;
        if (FIT_HEIGHT.width() < bestFit.width()) bestFit = FIT_HEIGHT;

        resize(bestFit);
        move((parent->width() - bestFit.width()) / 2, (minTopShift - bestFit.height()) / 2);
    }
}
