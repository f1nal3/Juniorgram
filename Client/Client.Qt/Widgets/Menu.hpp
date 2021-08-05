#pragma once
#include <QWidget>
#include <memory>

#include "MenuItem.hpp"

/**
 * @class Menu
 * @brief A  menu
 */
class Menu : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for menu
     * @param parent Parent widget
     * @param st Styles
     */
    explicit Menu(QWidget* parent = nullptr, const Style::Menu& st = st::defaultMenu);

    /**
     * @breif Adds widget to a menu
     * @pararm widget
     * @return Action of widget
     */
    QAction* addAction(QWidget* widget);

    /**
     * @brief Adds menu item callbackData to menu
     * @param callbackData MenuItem callbackData
     * @return Action of menu item
     */
    QAction* addAction(std::unique_ptr<ItemBase> callbackData);
    /**
     * @brief Adds action to menu
     * @param text Text of item
     * @param callback Callback function for item
     * @param icon Icon for item
     * @param iconOver Over icon for item
     * @return Action of menu item
     */
    QAction* addAction(const QString& text, std::function<void()>&& callback, const Style::icon* icon = nullptr,
                       const Style::icon* iconOver = nullptr);
    /**
     * @brief Adds action to menu
     * @param action Action itself
     * @param icon Icon
     * @param iconOver OverIcon
     * @return Action of menu item
     */
    QAction* addAction(QAction* action, const Style::icon* icon = nullptr, const Style::icon* iconOver = nullptr);
    /**
     * @brief Menu separator
     * @return Action of menu item
     */
    QAction* addSeparator();

    /**
     * @brief Clear menu
     */
    void clearActions();
    /**
     * @brief Is menu empty
     * @return Is menu empty
     */
    [[nodiscard]] bool empty() const;

    /**
     * @brief Sets callback on item selection
     * @param callback Callback function
     */
    void setActivatedCallback(std::function<void(const CallbackData& data)> callback) { _activatedCallback = std::move(callback); }
    /**
     * @brief Sets callback on item trigger
     * @param callback
     */
    void setTriggeredCallback(std::function<void(const CallbackData& data)> callback) { _triggeredCallback = std::move(callback); }

    void setMouseMoveDelegate(std::function<void(QPoint globalPosition)> delegate) { _mouseMoveDelegate = std::move(delegate); }
    void handleMouseMove(QPoint globalPosition);

    void setMousePressDelegate(std::function<void(QPoint globalPosition)> delegate) { _mousePressDelegate = std::move(delegate); }
    void handleMousePress(QPoint globalPosition);

    void setMouseReleaseDelegate(std::function<void(QPoint globalPosition)> delegate) { _mouseReleaseDelegate = std::move(delegate); }
    void handleMouseRelease(QPoint globalPosition);

    /**
     * @brief Finds currently selected
     * @return
     */
    ItemBase* findSelectedAction() const;
    /**
     * @brief removes selection
     */
    void clearSelection();
    void setSelected(int32_t selected);
    void updateSelected(const QPoint& globalPosition);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void hideEvent(QHideEvent* hideEvent) override;
    void paintEvent(QPaintEvent* paintEvent) override;

private:
    const Style::Menu& _st;

    std::function<void(QPoint globalPosition)> _mouseMoveDelegate;
    std::function<void(QPoint globalPosition)> _mousePressDelegate;
    std::function<void(QPoint globalPosition)> _mouseReleaseDelegate;

    std::function<void(const CallbackData& data)> _activatedCallback;
    std::function<void(const CallbackData& data)> _triggeredCallback;

    std::vector<QAction*>                  _actions;
    std::vector<std::unique_ptr<ItemBase>> _items;
};
