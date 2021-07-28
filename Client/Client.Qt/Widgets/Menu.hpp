#pragma once
#include <QWidget>
#include <memory>

#include "MenuItem.hpp"

/**
 * @class Menu
 * @brief A simple menu
 */
class Menu : public QWidget
{
    Q_OBJECT
public:
    explicit Menu(QWidget* parent = nullptr, const Style::Menu& st = st::defaultMenu);

    QAction* addAction(std::unique_ptr<ItemBase> widget);
    QAction* addAction(const QString& text, std::function<void()>&& callback, const Style::icon* icon = nullptr,
                       const Style::icon* iconOver = nullptr);
    QAction* addAction(QAction* action, const Style::icon* icon = nullptr, const Style::icon* iconOver = nullptr);
    QAction* addSeparator();

    void clearActions();
    bool empty() const;

    void setActivatedCallback(std::function<void(const CallbackData& data)> callback) { _activatedCallback = std::move(callback); }
    void setTriggeredCallback(std::function<void(const CallbackData& data)> callback) { _triggeredCallback = std::move(callback); }

    void setMouseMoveDelegate(std::function<void(QPoint globalPosition)> delegate) { _mouseMoveDelegate = std::move(delegate); }
    void handleMouseMove(QPoint globalPosition);

    void setMousePressDelegate(std::function<void(QPoint globalPosition)> delegate) { _mousePressDelegate = std::move(delegate); }
    void handleMousePress(QPoint globalPosition);

    void setMouseReleaseDelegate(std::function<void(QPoint globalPosition)> delegate) { _mouseReleaseDelegate = std::move(delegate); }
    void handleMouseRelease(QPoint globalPosition);

    ItemBase* findSelectedAction() const;
    void      clearSelection();
    void      setSelected(int32_t selected);
    void      updateSelected(const QPoint& globalPosition);

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
