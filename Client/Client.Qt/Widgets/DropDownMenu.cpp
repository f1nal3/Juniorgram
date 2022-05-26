#include <QPainter>

#include "DropDownMenu.hpp"

DropDownMenu::DropDownMenu(QWidget *parent)
    : AbstractDropDownMenu(parent)
{
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::Popup |
                   Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

void DropDownMenu::showDropDownMenu(QPoint position)
{
    setGeometry(QRect(position, QSize(width(), height())));
    QWidget::show();
}

void DropDownMenu::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    adjustSize();

    QRect rectRegion;

    rectRegion.setX(rect().x());
    rectRegion.setY(rect().y());
    rectRegion.setHeight(rect().height());
    rectRegion.setWidth(rect().width());


    painter.setBrush(QBrush(QColor(Qt::black)));
    painter.setPen(Qt::NoPen);

    painter.drawRect(rectRegion);
}
