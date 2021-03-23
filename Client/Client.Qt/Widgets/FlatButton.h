#pragma once
#ifndef JUNIORGRAM_FLATBUTTON_H
#define JUNIORGRAM_FLATBUTTON_H

#include "pch.h"

class FlatButton : public QPushButton
{
public:
    explicit FlatButton(const QString& text, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif  // JUNIORGRAM_FLATBUTTON_H
