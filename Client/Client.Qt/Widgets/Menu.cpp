#include "Menu.hpp"

#include <QAction>
#include <QWidgetAction>
#include <QtEvents>
#include <Style/Styles.hpp>
#include <memory>

#include "ReactionLayout.hpp"

//QAction* CreateAction(QWidget* actionparent, const QString& text, std::function<void()>&& callback)
//{
//    const auto action = std::make_shared<QAction>(text, actionparent);
//    QWidget::connect(action.get(), &QAction::triggered, action.get(), std::move(callback), Qt::QueuedConnection);
//    return action.get();
//}
QAction* CreateAction(QWidget* actionparent, const QString& text, std::function<void()>&& callback)
{
    const auto action = new QAction(text, actionparent);
    actionparent->connect(action, &QAction::triggered, action, std::move(callback), Qt::QueuedConnection);
    return action;
}
Menu::Menu(QWidget* parent, const Style::Menu& st) : QWidget(parent), _st(st)
{
    setMouseTracking(true);
    setMaximumWidth(st.widthMax);
}

void Menu::paintEvent(QPaintEvent* paintEvent) { QWidget::paintEvent(paintEvent); }

//QAction* Menu::addAction(QWidget* widget)
//{
//    const auto action = std::make_shared<QWidgetAction>(this);
//    action->setDefaultWidget(widget);
//    _actions.emplace_back(action.get());
//
//    const auto item = std::make_shared<WidgetItem>(this, action.get());
//
//    const int top = _items.empty() ? 0 : _items.back()->y() + _items.back()->contentHeight();
//    item->move(0, top);
//    item->show();
//
//    connect(dynamic_cast<ReactionLayout*>(widget), &ReactionLayout::clicked, [this](const CallbackData& callbackData) {
//        if (this->_triggeredCallback)
//        {
//            this->_triggeredCallback(callbackData);
//        }
//    });
//
//    _items.push_back(std::unique_ptr<ItemBase>(item.get()));
//    resize(width(), _items.empty() ? 0 : _items.back()->y() + widget->height());
//
//    return action.get();
//}
QAction* Menu::addAction(QWidget* widget)
{
    const auto action = new QWidgetAction(this);
    action->setDefaultWidget(widget);
    _actions.emplace_back(action);

    const auto item = new WidgetItem(this, action);

    const int top = _items.empty() ? 0 : _items.back()->y() + _items.back()->contentHeight();
    item->move(0, top);
    item->show();

    connect(dynamic_cast<ReactionLayout*>(widget), &ReactionLayout::clicked, [=](const CallbackData& callbackData) {
        if (_triggeredCallback)
        {
            _triggeredCallback(callbackData);
        }
    });

    _items.push_back(std::unique_ptr<ItemBase>(item));
    resize(width(), _items.empty() ? 0 : _items.back()->y() + widget->height());

    return action;
}

QAction* Menu::addAction(const QString& text, std::function<void()>&& callback, const Style::icon* icon, const Style::icon* iconOver)
{
    auto action = CreateAction(this, text, std::move(callback));
    return addAction(action, icon, iconOver);
}

QAction* Menu::addAction(QAction* action, const Style::icon* icon, const Style::icon* iconOver)
{
    if (action->isSeparator())
    {
        return addSeparator();
    }
    auto item = std::make_unique<MenuItem>(this, _st, action, icon, iconOver ? iconOver : icon);
    return addAction(std::move(item));
}

//QAction* Menu::addSeparator()
//{
//    const auto separator = std::make_unique<QAction>(this);
//    separator->setSeparator(true);
//    auto item = std::make_unique<Separator>(this, _st);
//    return addAction(std::move(item));
//}
QAction* Menu::addSeparator()
{
    const auto separator = new QAction(this);
    separator->setSeparator(true);
    auto item = std::make_unique<Separator>(this, _st);
    return addAction(std::move(item));
}
    /*QAction* Menu::addAction(std::unique_ptr<ItemBase> widget)
{
    const auto action = widget->action();
    _actions.emplace_back(action);

    widget->setParent(this);

    const int top = _items.empty() ? 0 : _items.back()->y() + _items.back()->contentHeight();
    widget->move(0, top);
    widget->show();

    widget->setIndex(int32_t(_items.size()));

    connect(widget.get(), &ItemBase::clicked, [this](const CallbackData& callbackData) {
        if (this->_triggeredCallback)
        {
            this->_triggeredCallback(callbackData);
        }
    });

    connect(widget.get(), &ItemBase::selected, [this](const CallbackData& callbackData) {
        if (this->_activatedCallback)
        {
            this->_activatedCallback(callbackData);
        }
    });

    _items.push_back(std::move(widget));
    resize(width(), _items.empty() ? 0 : _items.back()->y() + _items.back()->height());

    return action;
}*/
QAction* Menu::addAction(std::unique_ptr<ItemBase> widget)
{
    const auto action = widget->action();
    _actions.emplace_back(action);
    widget->setParent(this);
    const int top = _items.empty() ? 0 : _items.back()->y() + _items.back()->contentHeight();
    widget->move(0, top);
    widget->show();

    widget->setIndex(int32_t(_items.size()));

    connect(widget.get(), &ItemBase::clicked, [=](const CallbackData& callbackData) {
        if (_triggeredCallback)
        {
            _triggeredCallback(callbackData);
        }
    });

    connect(widget.get(), &ItemBase::selected, [=](const CallbackData& callbackData) {
        if (_activatedCallback)
        {
            _activatedCallback(callbackData);
        }
    });

    _items.push_back(std::move(widget));
    resize(width(), _items.empty() ? 0 : _items.back()->y() + _items.back()->height());
    return action;
}
void Menu::hideEvent(QHideEvent* hideEvent)
{
    if (parentWidget())
    {
        parentWidget()->hide();
    }
    QWidget::hideEvent(hideEvent);
}
bool Menu::empty() const { return _items.empty(); }

void Menu::clearActions()
{
    _items.clear();
    for (auto action : _actions)
    {
        if (action->parent() == this) delete action;
    }
}

void Menu::mousePressEvent(QMouseEvent* e) { handleMousePress(e->globalPos()); }

void Menu::mouseReleaseEvent(QMouseEvent* e) { handleMouseRelease(e->globalPos()); }

void Menu::handleMousePress(QPoint globalPosition)
{
    handleMouseMove(globalPosition);
    if (_mousePressDelegate)
    {
        _mousePressDelegate(globalPosition);
    }
}

void Menu::handleMouseRelease(QPoint globalPosition) const
{
    if (!rect().contains(mapFromGlobal(globalPosition)) && _mouseReleaseDelegate)
    {
        _mouseReleaseDelegate(globalPosition);
    }
}
void Menu::mouseMoveEvent(QMouseEvent* event) { handleMouseMove(event->globalPos()); }

void Menu::handleMouseMove(QPoint globalPosition)
{
    const auto inner         = rect();
    const auto localPosition = mapFromGlobal(globalPosition);
    if (inner.contains(localPosition))
    {
        updateSelected(globalPosition);
    }
    else
    {
        clearSelection();
        if (_mouseMoveDelegate)
        {
            _mouseMoveDelegate(globalPosition);
        }
    }
}

void Menu::clearSelection() { setSelected(-1); }

ItemBase* Menu::findSelectedAction() const
{
    for (const auto& item : _items)
    {
        if (item->isSelected()) return item.get();
    }
    return nullptr;
}

void Menu::setSelected(int32_t selected)
{
    if (selected >= int32_t(_items.size()))
    {
        selected = -1;
    }

    if (const auto selectedItem = findSelectedAction())
    {
        if (selectedItem->index() == selected)
        {
            return;
        }
        selectedItem->setSelected(false);
    }
    if (selected >= 0)
    {
        _items[selected]->setSelected(true);
    }
}

void Menu::updateSelected(const QPoint& globalPosition) const
{
    const auto p = mapFromGlobal(globalPosition);
    for (const auto& item : _items)
    {
        const auto widgetRect = QRect(item->pos(), item->size());
        if (widgetRect.contains(p))
        {
            item->setSelected(true);
            break;
        }
    }
}
