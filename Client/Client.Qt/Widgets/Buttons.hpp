#pragma once

#include <Style/Styles.hpp>

#include "AbstractButton.hpp"

/**
 * @class FlatButton
 * @brief Simple flat button
 */
class FlatButton : public AbstractButton
{
public:
    /**
     * @brief Constructor for a flat button
     * @param parent Parent widget
     * @param text Button text
     * @param st Button styles
     */
    explicit FlatButton(QWidget* parent, const QString& text, const Style::FlatButton& st = st::defaultFlatButton);
    /**
     * @brief Sets a new text for a button
     * @param text New text
     */
    void setText(const QString& text);

protected:
    /**
     * @brief Handle paint
     * @param event Event
     */
    void paintEvent(QPaintEvent* event) override;

    /**
     * @brief Handle state changing
     * @param was State before
     * @param source who has done this
     */
    void onStateChanged(State was, StateChanger source) override
    {
        Q_UNUSED(was)
        Q_UNUSED(source)
        update();
    }

private:
    QString                  _text;
    const Style::FlatButton& _st;
};

/**
 * @class LinkButton
 * @brief "Button" to send user to link
 */
class LinkButton : public AbstractButton
{
public:
    /// Constructor for LinkButton
    explicit LinkButton(QWidget* parent, const QString& text, const Style::LinkButton& st = st::defaultLinkButton);

    /// Set text
    void setText(const QString& text);

protected:
    /// Handle paint
    void paintEvent(QPaintEvent* event) override;
    /// Update on state change
    void onStateChanged(State was, StateChanger source) override
    {
        Q_UNUSED(was)
        Q_UNUSED(source)
        update();
    }

private:
    QString                  _text;
    const Style::LinkButton& _st;
};

/**
 * @class IconButton
 * @brief Button with icons
 */
class IconButton : public AbstractButton
{
public:
    /**
     * @brief Constructor for an icon button
     * @param parent Parent widget
     * @param text Button text
     * @param st Button styles
     */
    explicit IconButton(QWidget* parent, QString text, const Style::IconButton& st = st::defaultIconButton);
    /**
     * @brief Sets a new text for a button
     * @param text New text
     */
    void setText(const QString& text);

    /**
     * @brief Set new icon
     * @param icon new icon
     */
    void setIcon(const Style::icon* icon);

    /**
     * @brief Update widget if something has changed
     */
    void updateWidget();

protected:
    /**
     * @brief Handle paint
     * @param event Event
     */
    void paintEvent(QPaintEvent* event) override;

    /**
     * @brief Handle state changing
     * @param was State before
     * @param source who has done this
     */
    void onStateChanged(State was, StateChanger source) override
    {
        Q_UNUSED(was)
        Q_UNUSED(source)
        update();
    }

private:
    QString _text;

    const Style::icon*       _iconOverride = nullptr;
    const Style::IconButton& _st;
};
