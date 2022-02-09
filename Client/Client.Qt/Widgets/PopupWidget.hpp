#pragma once
#include <QWidget>
#include <QLabel>

#include "Menu.hpp"

/**
 * @class PopupWidget
 * @brief Widget to show popup menu(modal)
 */
class PopupWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for popup widget
     * @param parent Parent popup
     */
    explicit PopupWidget(QWidget* parent = nullptr);

    /**
     * @brief Set owned menu
     * @param menu Menu
     */
    void setMenu(std::unique_ptr<Menu> menu);
    /**
     * @brief Destroy window on hide?
     * @param deleteOnHide Delete on hide
     */
    void setDeleteOnHide(bool deleteOnHide);
    /**
     * @brief Show popup menu
     * @param globalPoint Global globalPoint
     */

    void setMessage(QString text);

    void popup(const QPoint& globalPoint);

protected:
    /**
     * @brief Handle drawing
     */
    void paintEvent(QPaintEvent* event) override;
    /**
     * @brief Handle hide
     * @param event Event info
     */
    void hideEvent(QHideEvent* event) override;

private:
    std::unique_ptr<QLabel> _messageText;
    std::unique_ptr<Menu> _innerMenu;
    bool                  _deleteOnHide = false;
};

void messageOut(std::shared_ptr<PopupWidget> popupWidget, QString text);
