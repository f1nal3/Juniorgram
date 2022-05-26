#pragma once

#include <QGridLayout>

#include "AbstractDropDownMenu.hpp"

class DropDownMenu : public AbstractDropDownMenu
{
public:
    explicit DropDownMenu(QWidget* parent = nullptr);
    ~DropDownMenu() override = default;

public slots:
    void showDropDownMenu(QPoint position) override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QGridLayout _layout;
};
