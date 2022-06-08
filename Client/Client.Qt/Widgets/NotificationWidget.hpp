#pragma once
#include <QWidget>
#include <QLabel>

#include "Menu.hpp"

/**
 * @class NotificationWidget
 * @brief Widget to show notification menu(modal)
 */
class NotificationWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for notification widget
     * @param parent Parent notification
     */
    explicit NotificationWidget(QWidget* parent = nullptr);

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
     * @brief Show notification menu
     * @param globalPoint Global globalPoint
     */

    void setMessage(QString text);

    void notification(const QPoint& globalPoint);

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
    std::unique_ptr<Menu>   _innerMenu    = nullptr;
    bool                    _deleteOnHide = false;
};

void messageOut(std::shared_ptr<NotificationWidget> notificationWidget, QString text);
