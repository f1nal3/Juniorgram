#pragma once
#include <utility>

#include "pch.hpp"

class AbstractButton : public QWidget
{
public:
    explicit AbstractButton(QWidget* parent = nullptr);
    virtual void clearState();

    bool isOver() const { return _state & StateFlag::Over; }
    bool isDown() const { return _state & StateFlag::Down; }
    bool isDisabled() const { return _state & StateFlag::Disabled; }

    void setDisabled(bool disabled = true);
    void setClickCallback(std::function<void()> callback) { _clickCallback = std::move(callback); }
    void setPointerCursor(bool enablePointerCursor);

    void clicked(Qt::KeyboardModifiers modifiers, Qt::MouseButton button);

protected:
    void enterEvent(QEvent* e) override;
    void leaveEvent(QEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

protected:
    enum class StateFlag
    {
        None     = 0,
        Over     = (1 << 0),
        Down     = (1 << 1),
        Disabled = (1 << 2)
    };
    using State = QFlags<StateFlag>;

    State state() const { return _state; }
    enum class StateChanger
    {
        ByUser  = 0x0,
        ByPress = 0x1,
        ByHover = 0x2,
    };

    virtual void onStateChanged(State was, StateChanger source)
    {
        Q_UNUSED(was);
        Q_UNUSED(source);
    }

    void setOver(bool over, StateChanger source = StateChanger::ByUser);

private:
    void                  checkIfOver(QPoint localPos);
    void                  updateCursor();
    Qt::KeyboardModifiers _modifiers;
    bool                  _pointerCursor = false;
    State                 _state;
    std::function<void()> _clickCallback;
};