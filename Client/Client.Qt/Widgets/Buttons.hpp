#pragma once

#include "AbstractButton.hpp"
#include "Style/StyleBasic.hpp"

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
    void paintEvent(QPaintEvent* event) override;

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

class LinkButton : public AbstractButton
{
public:
    explicit LinkButton(QWidget* parent, const QString& text, const Style::LinkButton& st = st::defaultLinkButton);

    void setText(const QString& text);

protected:
    void paintEvent(QPaintEvent* event) override;
    void onStateChanged(State was, StateChanger source) override
    {
        Q_UNUSED(was);
        Q_UNUSED(source);
        update();
    }

private:
    QString                  _text;
    const Style::LinkButton& _st;
};
