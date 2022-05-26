#include "AbstractDropDownMenu.hpp"

AbstractDropDownMenu::AbstractDropDownMenu(QWidget *parent)
    : QWidget(parent)
{
}

void AbstractDropDownMenu::setDropDownMenu(std::unique_ptr<QMenu> newMenu)
{
    _menu = std::move(newMenu);

    _menu->setParent(this);

    _menu->setStyleSheet("QMenu"
                         "{"
                         "background-color: transparent;"
                         "border: 0px;"
                         "color: white"
                         "}"
                         "QMenu::item:selected"
                         "{"
                         "background: rgba(100, 100, 100, 150);"
                         "}");
}

void AbstractDropDownMenu::showDropDownMenu(QPoint position)
{
    setGeometry(QRect(position, QSize(width(), height())));
    QWidget::show();
}

const QMenu& AbstractDropDownMenu::getDropDownMenu()
{
    return *_menu;
}
