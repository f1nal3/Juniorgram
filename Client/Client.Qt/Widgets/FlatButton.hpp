#pragma once

#include "AbstractButton.hpp"
#include "Style/StyleBasic.hpp"
#include "pch.hpp"

class FlatButton : public AbstractButton
{
public:
    explicit FlatButton(QWidget* parent, const QString& text,
                        const Style::FlatButton& st = st::defaultFlatButton);
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
    const Style::FlatButton& _st;
};

class LinkButton : public AbstractButton
{
public:
    explicit LinkButton(QWidget* parent, const QString& text,
                        const Style::LinkButton& st = st::defaultLinkButton);

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
