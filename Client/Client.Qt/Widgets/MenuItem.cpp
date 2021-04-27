#include "MenuItem.hpp"

#include "Style/Style.hpp"
MenuItem::MenuItem(const QString& text, QAction* action, QWidget* parent) : QWidget(parent)
{
    innerText = text;
    mAction   = action;
    setFixedSize(parent->width() - 1, Style::valueDPIScale(30));
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
}
QSize MenuItem::sizeHint() const { return QWidget::sizeHint(); }
void MenuItem::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);
    QColor color = QColor(0x32, 0x32, 0x32);
    if (hit)
    {
        color = color.lighter(175);
    }
    p.setBrush(color);
    p.drawRect(rect());
    p.setBrush(Qt::red);
    p.setPen(Qt::white);

    p.drawText(5, 5, 245, 20, 0, innerText);
    QWidget::paintEvent(event);
}
void MenuItem::mouseMoveEvent(QMouseEvent* e)
{
    Q_UNUSED(e)
    if (underMouse()) hit = true;
}
void MenuItem::leaveEvent(QEvent* e)
{
    Q_UNUSED(e)
    hit = false;
}
void MenuItem::mouseReleaseEvent(QMouseEvent* e)
{
    Q_UNUSED(e)
    mAction->trigger();
    parentWidget()->hide();
    QWidget::mouseReleaseEvent(e);
}
