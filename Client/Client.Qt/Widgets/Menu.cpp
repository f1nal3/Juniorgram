#include "Menu.hpp"

#include <QAction>
#include <QtEvents>

#include "Style/StyleBasic.hpp"

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

QAction* Menu::addAction(const QString& text, std::function<void()>&& callback, const Style::icon* icon, const Style::icon* iconOver)
{
    auto action = CreateAction(this, text, std::move(callback));
    return addAction(std::move(action), icon, iconOver);
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

QAction* Menu::addSeparator()
{
    const auto separator = new QAction(this);
    separator->setSeparator(true);
    auto item = std::make_unique<Separator>(this, _st);
    return addAction(std::move(item));
}

QAction* Menu::addAction(std::unique_ptr<ItemBase> widget)
{
    const auto action = widget->action();
    _actions.emplace_back(action);

    widget->setParent(this);

    const int top = _items.empty() ? 0 : _items.back()->y() + _items.back()->height();
    widget->move(0, top);
    widget->show();

    widget->setIndex(int32_t(_items.size()));

    connect(widget.get(), &ItemBase::clicked, [=](const CallbackData& data) {
        if (_triggeredCallback)
        {
            _triggeredCallback(data);
        }
    });

    connect(widget.get(), &ItemBase::selected, [=](const CallbackData& data) {
        if (_activatedCallback)
        {
            _activatedCallback(data);
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

void Menu::handleMouseRelease(QPoint globalPosition)
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

void Menu::updateSelected(const QPoint& globalPosition)
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
