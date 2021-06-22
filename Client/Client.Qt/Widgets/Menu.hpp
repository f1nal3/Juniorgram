#pragma once
#include "MenuItem.hpp"
#include "pch.hpp"

class Menu : public QWidget
{
    Q_OBJECT
public:
    explicit Menu(QWidget* parent = nullptr);
    void addAction(const QString& text, std::function<void()>&& callback);

protected:
    void hideEvent(QHideEvent* event) override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    std::vector<MenuItem*> _items;
};
