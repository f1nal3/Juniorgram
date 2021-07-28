#include "MenuItem.hpp"

#include <QAction>
#include <QPainter>
#include <QtEvents>
#include <utility>

ItemBase::ItemBase(QWidget* parent, const Style::Menu&) : AbstractButton(parent)
{
    setClickCallback([=]() {
        if (isEnabled())
        {
            action()->trigger();
            clicked({action(), y(), _index, isSelected()});
        }
    });
    installEventFilter(this);
    setMouseTracking(true);
}

void ItemBase::setSelected(bool selected)
{
    if (!isEnabled()) return;
    if (_selected != selected)
    {
        setMouseTracking(!selected);
        _selected = selected;
        update();
    }
    if (_selected)
    {
        this->selected({action(), y(), _index, isSelected()});
    }
}

bool ItemBase::isSelected() const { return _selected; }

int ItemBase::index() const { return _index; }

void ItemBase::setIndex(int32_t index) { _index = index; }

bool ItemBase::eventFilter(QObject*, QEvent* e)
{
    const auto type = e->type();
    if (((type == QEvent::Leave) || (type == QEvent::Enter) || (type == QEvent::MouseMove)) && (action() && action()->isEnabled()))
    {
        setSelected(e->type() != QEvent::Leave);
        return true;
    }
    else if ((type == QEvent::MouseButtonRelease) && isEnabled() && isSelected())
    {
        const auto point = mapFromGlobal(QCursor::pos());
        if (!rect().contains(point))
        {
            setSelected(false);
            return true;
        }
    }
    return false;
}

Separator::Separator(QWidget* parent, const Style::Menu& st, QAction* action)
    : ItemBase(parent, st),
      _lineWidth(st.separatorWidth),
      _padding(st.separatorPadding),
      _fg(st.separatorFg),
      _bg(st.itemBg),
      _height(_padding.top() + _lineWidth + _padding.bottom()),
      _action(action)
{
    resize(parent->width(), contentHeight());
}

QAction* Separator::action() const { return _action; }

bool Separator::isEnabled() const { return false; }

int Separator::contentHeight() const { return _height; }

void Separator::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    p.fillRect(0, 0, width(), _height, _bg);
    p.fillRect(_padding.left(), _padding.top(), width() - _padding.left() - _padding.right(), _lineWidth, _fg);
}

MenuItem::MenuItem(QWidget* parent, const Style::Menu& st, QAction* action, const Style::icon* icon, const Style::icon* iconOver)
    : ItemBase(parent, st),
      _action(action),
      _st(st),
      _icon(icon),
      _iconOver(iconOver),
      _height(_st.itemPadding.top() + _st.itemFont->height + _st.itemPadding.bottom())
{
    setPointerCursor(false);

    resize(parent->width(), contentHeight());
    processAction();

    connect(_action, &QAction::changed, [=] { processAction(); });
}

int32_t MenuItem::contentHeight() const { return _height; }

bool MenuItem::isEnabled() const { return _action->isEnabled(); }

QAction* MenuItem::action() const { return const_cast<QAction*>(_action); }

void MenuItem::paintEvent(QPaintEvent*)
{
    QPainter   p(this);
    const auto enabled  = _action->isEnabled();
    const auto selected = isSelected();
    if (selected && _st.itemBgOver->color.alpha() < 255)
    {
        p.fillRect(0, 0, width(), _height, _st.itemBg);
    }
    p.fillRect(0, 0, width(), _height, selected ? _st.itemBgOver : _st.itemBg);

    p.setPen(isSelected() ? _st.itemFgOver : (enabled ? _st.itemFg : _st.itemFgDisabled));
    p.setFont(_st.itemFont);
    const auto textRect = QRect(_st.itemPadding.left(), _st.itemPadding.top(), _textWidth, _st.itemFont->height);
    p.drawText(textRect, _text, Style::al_left);
}

void MenuItem::processAction()
{
    if (_action->text().isEmpty())
    {
        _text.clear();
        return;
    }
    _text                 = _st.itemFont->elided(_action->text(), _st.widthMax);
    const auto  textWidth = _st.itemFont->width(_text);
    const auto& padding   = _st.itemPadding;

    const auto goodWidth = padding.left() + textWidth + padding.right();

    const auto w = std::clamp(goodWidth, _st.widthMin, _st.widthMax);
    _textWidth   = w - (goodWidth - textWidth);
    update();
}