#pragma once

#include "pch.h"

class FlatButton : public QPushButton
{
public:
    explicit FlatButton(const QString& text, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
};
