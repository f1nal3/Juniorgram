#pragma once

#include <QWidget>
#include "Menu.hpp"

class IDropDownMenu
{
public:
    virtual ~IDropDownMenu() = default;

    virtual void setDropDownMenu(std::unique_ptr<Menu> newMenu) = 0;
    virtual void showDropDownMenu(QPoint position) = 0;
};

class AbstractDropDownMenu : public QWidget, public IDropDownMenu
{
    Q_OBJECT

public:
    explicit AbstractDropDownMenu(QWidget *parent = nullptr);
    ~AbstractDropDownMenu() override = default;

    void setDropDownMenu(std::unique_ptr<Menu> newMenu) override;

    void showDropDownMenu(QPoint position) override;
protected:
    std::unique_ptr<Menu> _menu;
};
