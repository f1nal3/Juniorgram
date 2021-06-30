#pragma once
#include <QWidget>

#include "MenuItem.hpp"

class Menu : public QWidget
{
    Q_OBJECT
public:
    explicit Menu(QWidget* parent = nullptr);
    void addAction(const QString& text, std::function<void()>&& callback);

protected:
    void hideEvent(QHideEvent* hideEvent) override;
    void paintEvent(QPaintEvent* paintEvent) override;

private:
    std::vector<std::unique_ptr<MenuItem>> _items;
};
