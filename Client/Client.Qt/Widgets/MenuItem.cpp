#include "MenuItem.hpp"

#include <QAction>
#include <QPainter>
#include <utility>

#include "Style/StyleBasic.hpp"

ItemBase::ItemBase(QString text, QAction* action, QWidget* parent) : AbstractButton(parent), _text(std::move(text)), _action(action)
{
    setFont(st::defaultFont);

    resize(parent->width() - 1, Style::valueDPIScale(30));

    setMouseTracking(true);
    // setAttribute(Qt::WA_Hover);
    setClickCallback([=]() {
        action->trigger();
        parentWidget()->hide();
    });
}

void ItemBase::paintEvent(QPaintEvent* paintEvent)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    QColor color = QColor(0x32, 0x32, 0x32);
    if (isOver())
    {
        color = color.lighter(175);
    }
    painter.setBrush(color);
    painter.drawRect(rect());
    painter.setBrush(Qt::red);
    painter.setPen(Qt::white);

    const int TOP_X = (height() - fontMetrics().height()) / 2;
    const int SHIFT = (height() - 10);
    painter.drawText(10 + SHIFT, TOP_X, 245, fontMetrics().height(), 0, _text);
    QWidget::paintEvent(paintEvent);
}
