#include "AbstractDropDownMenu.hpp"

AbstractDropDownMenu::AbstractDropDownMenu(QWidget *parent)
    : QWidget(parent)
{
}

void AbstractDropDownMenu::setMenu(std::unique_ptr<Menu> newMenu)
{
    _menu = std::move(newMenu);
    _menu->setParent(this);
    _menu->setTriggeredCallback([this](const CallbackData&) { hide(); });
}

void AbstractDropDownMenu::showDropDownMenu(QPoint position)
{
    setGeometry(QRect(position, QSize(width(), height())));
    QWidget::show();
}
