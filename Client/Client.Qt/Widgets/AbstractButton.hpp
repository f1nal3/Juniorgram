#pragma once
#include <QWidget>
#include <utility>

/**
 * @class AbstractButton
 * @brief Class for button-like widgets
 */
class AbstractButton : public QWidget
{
public:
    /**
     * Constructor for button-like abstraction
     * @param parent Parent widget
     */
    explicit AbstractButton(QWidget* parent = nullptr);
    /**
     * @brief Clears state of button
     */
    virtual void clearState();

    /**
     * @return Is mouse over this widget?
     */
    [[nodiscard]] bool isOver() const { return _state & StateFlag::Over; }

    /**
     * @return Is mouse pressed on this widget?
     */
    [[nodiscard]] bool isDown() const { return _state & StateFlag::Down; }

    /**
     * @return Is button disable
     */
    [[nodiscard]] bool isDisabled() const { return _state & StateFlag::Disabled; }

    /**
     * @brief Enable/disable this button
     * @param disabled Enable/disable this button
     */
    void setDisabled(bool disabled = true);
    /**
     * @brief Sets a callback on button click
     * @param callback Callback function
     */
    void setClickCallback(std::function<void()> callback) { _clickCallback = std::move(callback); }
    /**
     * @brief Sets a pointer as a cursor over the button
     * @param enablePointerCursor Set a pointer as a cursor over the button
     */
    void setPointerCursor(bool enablePointerCursor);

    /**
     * @brief Triggers click event
     * @param modifiers Keyboard modifiers
     * @param button Mouse button
     */
    void clicked(Qt::KeyboardModifiers modifiers, Qt::MouseButton button);

protected:
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mouseMoveEvent(QMouseEvent* mouseEvent) override;
    void mousePressEvent(QMouseEvent* mouseEvent) override;
    void mouseReleaseEvent(QMouseEvent* mouseEvent) override;

protected:
    enum class StateFlag
    {
        None     = 0,
        Over     = (1 << 0),
        Down     = (1 << 1),
        Disabled = (1 << 2)
    };
    using State = QFlags<StateFlag>;

    /**
     * @brief Current button state
     * @return Button state
     */
    [[nodiscard]] State state() const { return _state; }
    enum class StateChanger
    {
        ByUser  = 0x0,
        ByPress = 0x1,
        ByHover = 0x2,
    };

    /**
     * @brief Triggers on state change
     * @param was State before
     * @param source State changer
     */
    virtual void onStateChanged(State was, StateChanger source)
    {
        Q_UNUSED(was);
        Q_UNUSED(source);
    }

    /**
     * @brief Set mouse over
     * @param over Over?
     * @param source By who/what?
     */
    void setOver(bool over, StateChanger source = StateChanger::ByUser);

private:
    void                  checkIfOver(QPoint localPos);
    void                  updateCursor();
    Qt::KeyboardModifiers _modifiers;
    bool                  _pointerCursor = false;
    State                 _state;
    std::function<void()> _clickCallback;
};
