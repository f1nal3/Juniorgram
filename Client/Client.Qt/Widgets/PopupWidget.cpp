#include "PopupWidget.hpp"

void PopupWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QColor inputField(0x35, 0x35, 0x35);
    painter.setBrush(inputField);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, width(), height());
}
void PopupWidget::hideEvent(QHideEvent* event)
{
    Q_UNUSED(event);
    deleteLater();
}
void PopupWidget::popup(const QPoint& point)
{
    move(point);
    if (innerMenu)
    {
        innerMenu->move(0, 10);
        setFixedSize(256, innerMenu->height() + 20);
        innerMenu->show();
    }
    show();
}
void PopupWidget::setMenu(Menu* menu)
{
    innerMenu = menu;
    innerMenu->setParent(this);
}
PopupWidget::PopupWidget(QWidget* parent) : QWidget(parent), innerMenu(nullptr)
{
    setWindowFlags(Qt::WindowFlags(Qt::FramelessWindowHint) | Qt::BypassWindowManagerHint |
                   Qt::Popup | Qt::NoDropShadowWindowHint);
    setMouseTracking(true);

    hide();
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFixedSize(256, 256);

    installEventFilter(this);
}
