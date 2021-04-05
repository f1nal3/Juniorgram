#include "Menu.h"
QAction* CreateAction(QWidget* actionparent, const QString& text, std::function<void()>&& callback)
{
    const auto action = new QAction(text, actionparent);
    actionparent->connect(action, &QAction::triggered, action, std::move(callback),
                          Qt::QueuedConnection);
    return action;
}

Menu::Menu(QWidget* parent) : QWidget(parent) {}

void Menu::paintEvent(QPaintEvent* event) { QWidget::paintEvent(event); }
void Menu::addAction(const QString& text, std::function<void()>&& callback)
{
    auto action = CreateAction(this, text, static_cast<std::function<void()>&&>(callback));
    auto Item   = std::make_unique<MenuItem>(text, action, this);
    int top     = items.empty() ? 0 : items.back()->y() + items.back()->height();
    Item->move(0, top);
    Item->show();
    items.push_back(std::move(Item));
    this->setFixedSize(256, items.back()->y() + items.back()->height());
}
void Menu::hideEvent(QHideEvent* event)
{
    if (parent())
    {
        parentWidget()->hide();
    }
    QWidget::hideEvent(event);
}
