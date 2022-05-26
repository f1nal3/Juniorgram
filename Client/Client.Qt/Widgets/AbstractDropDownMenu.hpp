#pragma once

#include <QWidget>
#include <QMenu>

class IDropDownMenu
{
public:
    virtual ~IDropDownMenu() = default;

    virtual void setDropDownMenu(std::unique_ptr<QMenu> newMenu) = 0;
    virtual void showDropDownMenu(QPoint position) = 0;
};

class AbstractDropDownMenu : public QWidget, public IDropDownMenu
{
    Q_OBJECT

public:
    explicit AbstractDropDownMenu(QWidget *parent = nullptr);
    ~AbstractDropDownMenu() override = default;

    void setDropDownMenu(std::unique_ptr<QMenu> newMenu) override;
    void showDropDownMenu(QPoint position) override;

    const QMenu& getDropDownMenu();

protected:
    std::unique_ptr<QMenu> _menu;
};
