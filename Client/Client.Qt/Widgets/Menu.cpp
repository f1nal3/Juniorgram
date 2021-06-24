#include "Menu.hpp"

QAction* CreateAction(QWidget* actionparent, const QString& text, std::function<void()>&& callback)
{
    const auto action = new QAction(text, actionparent);
    actionparent->connect(action, &QAction::triggered, action, std::move(callback),
                          Qt::QueuedConnection);
    return action;
}

Menu::Menu(QWidget* parent) : QWidget(parent) { _items = new std::vector<MenuItem*>; }

void Menu::paintEvent(QPaintEvent* paintEvent) { QWidget::paintEvent(paintEvent); }

Menu::~Menu()
{
    for (auto item : *_items)
        delete item;
    delete _items;
}

void Menu::addAction(const QString& text, std::function<void()>&& callback)
{
    auto action = CreateAction(this, text, static_cast<std::function<void()>&&>(callback));
    auto Item   = new MenuItem(text, action, this);
    int top     = _items->empty() ? 0 : _items->back()->y() + _items->back()->height();
    Item->move(0, top);
    Item->show();
    _items->push_back(std::move(Item));
    this->setFixedSize(256, _items->back()->y() + _items->back()->height());
}

void Menu::hideEvent(QHideEvent* hideEvent)
{
    if (parent())
    {
        parentWidget()->hide();
    }
    QWidget::hideEvent(hideEvent);
}
