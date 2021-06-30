#include "AbstractButton.hpp"

#include <QtEvents>

#include "Style/Style.hpp"

AbstractButton::AbstractButton(QWidget* parent) : QWidget(parent) { setMouseTracking(true); }

void AbstractButton::leaveEvent(QEvent* event)
{
    Q_UNUSED(event)
    setOver(false, StateChanger::ByHover);
}

void AbstractButton::enterEvent(QEvent* event)
{
    Q_UNUSED(event)
    setOver(true, StateChanger::ByHover);
}

void AbstractButton::setOver(bool over, AbstractButton::StateChanger source)
{
    Q_UNUSED(source);
    if (over && !(_state & StateFlag::Over))
    {
        auto was = _state;
        _state |= StateFlag::Over;
        onStateChanged(was, source);
    }
    else if (!over && (_state & StateFlag::Over))
    {
        auto was = _state;
        _state &= ~State(StateFlag::Over);
        onStateChanged(was, source);
    }
    updateCursor();
}

void AbstractButton::mouseMoveEvent(QMouseEvent* mouseEvent)
{
    if (rect().contains(mouseEvent->pos()))
    {
        setOver(true, StateChanger::ByHover);
    }
    else
    {
        setOver(false, StateChanger::ByHover);
    }
}

void AbstractButton::setDisabled(bool disabled)
{
    auto was = _state;
    if (disabled && !(_state & StateFlag::Disabled))
    {
        _state |= StateFlag::Disabled;
        onStateChanged(was, StateChanger::ByUser);
    }
    else if (!disabled && (_state & StateFlag::Disabled))
    {
        _state &= ~State(StateFlag::Disabled);
        onStateChanged(was, StateChanger::ByUser);
    }
}

void AbstractButton::mousePressEvent(QMouseEvent* mouseEvent)
{
    checkIfOver(mouseEvent->pos());
    if (mouseEvent->buttons() & Qt::LeftButton)
    {
        if ((_state & StateFlag::Over) && !(_state & StateFlag::Down))
        {
            auto was = _state;
            _state |= StateFlag::Down;
            onStateChanged(was, StateChanger::ByPress);

            mouseEvent->accept();
        }
    }
}

void AbstractButton::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
    if (_state & StateFlag::Down)
    {
        auto was = _state;
        _state &= ~State(StateFlag::Down);

        onStateChanged(was, StateChanger::ByPress);

        if (was & StateFlag::Over)
        {
            clicked(mouseEvent->modifiers(), mouseEvent->button());
        }
        else
        {
            setOver(false, StateChanger::ByHover);
        }
    }
}

void AbstractButton::checkIfOver(QPoint localPos)
{
    auto over = rect().contains(localPos);
    setOver(over, StateChanger::ByHover);
}

void AbstractButton::updateCursor()
{
    auto pointerCursor = _pointerCursor && (_state & StateFlag::Over);
    setCursor(pointerCursor ? Style::cur_pointer : Style::cur_default);
}

void AbstractButton::setPointerCursor(bool enablePointerCursor)
{
    if (enablePointerCursor != _pointerCursor)
    {
        _pointerCursor = enablePointerCursor;
        updateCursor();
    }
}

void AbstractButton::clicked(Qt::KeyboardModifiers modifiers, Qt::MouseButton button)
{
    _modifiers = modifiers;
    if (button == Qt::LeftButton)
    {
        if (auto callback = _clickCallback)
        {
            callback();
        }
    }
}

void AbstractButton::clearState()
{
    auto was = _state;
    _state   = StateFlag::None;
    onStateChanged(was, StateChanger::ByUser);
}
