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

    void setMenu(std::unique_ptr<Menu> menu);
    void setDeleteOnHide(bool deleteOnHide);
    void popup(const QPoint& point);

protected:
    void paintEvent(QPaintEvent* event) override;
    void hideEvent(QHideEvent* event) override;

private:
    std::unique_ptr<Menu> _innerMenu;
    bool                  _deleteOnHide = false;
};
