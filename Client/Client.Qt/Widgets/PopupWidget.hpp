#pragma once
#include <QWidget>

#include "Menu.hpp"

/**
 * @class PopupWidget
 */
class PopupWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PopupWidget(QWidget* parent = nullptr);

    void setMenu(Menu* menu);

protected:
    void paintEvent(QPaintEvent* event) override;
    void hideEvent(QHideEvent* event) override;

public:
    void popup(const QPoint& point);

private:
    Menu* _innerMenu;
};
