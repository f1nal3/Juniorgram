#pragma once

#include "pch.hpp"
#include "AbstractButton.hpp"
#include "Style/style_basic.h"

class FlatButton : public AbstractButton
{
public:
    explicit FlatButton(QWidget* parent, const QString& text,const Style::FlatButton &st = st::defaultFlatButton);
    void setText(const QString& text);

protected:
    void paintEvent(QPaintEvent* event) override;

    void onStateChanged(State was, StateChanger source) override{
        Q_UNUSED(was);
        Q_UNUSED(source);
        update();
    }
private:
    QString _text;
    const Style::FlatButton &_st;
};
