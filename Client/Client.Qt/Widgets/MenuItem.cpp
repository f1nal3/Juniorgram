#include "MenuItem.hpp"

#include "Style/Style.hpp"

MenuItem::MenuItem(const QString& text, QAction* action, QWidget* parent) : QWidget(parent)
{
    innerText = text;
    mAction   = action;

    auto font = QFont("Noto Sans", 12);
    font.setPixelSize(Style::valueDPIScale(12));
    setFont(font);

    setFixedSize(parent->width() - 1, Style::valueDPIScale(30));
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
}

QSize MenuItem::sizeHint() const { return QWidget::sizeHint(); }

void MenuItem::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    QColor color = QColor(0x32, 0x32, 0x32);
    if (hit)
    {
        color = color.lighter(175);
    }
    painter.setBrush(color);
    painter.drawRect(rect());
    painter.setBrush(Qt::red);
    painter.setPen(Qt::white);

    const int TOP_X = (height() - fontMetrics().height()) / 2;
    const int SHIFT = (height() - 10);
    painter.drawText(10 + SHIFT, TOP_X, 245, fontMetrics().height(), 0, innerText);
    QWidget::paintEvent(event);
}

void MenuItem::mouseMoveEvent(QMouseEvent* event)
{
    Q_UNUSED(event)
    if (underMouse()) hit = true;
}

void MenuItem::leaveEvent(QEvent* event)
{
    Q_UNUSED(event)
    hit = false;
}

void MenuItem::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event)
    mAction->trigger();
    parentWidget()->hide();
    QWidget::mouseReleaseEvent(event);
}
