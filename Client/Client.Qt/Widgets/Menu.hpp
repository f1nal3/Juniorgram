#pragma once
#include "MenuItem.hpp"
#include "pch.hpp"

class Menu : public QWidget
{
    Q_OBJECT
public:
    explicit Menu(QWidget* parent = nullptr);
    ~Menu();
    void addAction(const QString& text, std::function<void()>&& callback);

protected:
    void hideEvent(QHideEvent* hideEvent) override;
    void paintEvent(QPaintEvent* paintEvent) override;

private:
    std::vector<MenuItem*> *_items;
};
