#pragma once
#ifndef JUNIORGRAMUI_INPUT_FIELDS_H
#define JUNIORGRAMUI_INPUT_FIELDS_H

#include "Style/Style.h"
#include "pch.h"

class FlatInput : public QLineEdit
{
    Q_OBJECT
public:
    explicit FlatInput(QWidget* parent = nullptr);
    explicit FlatInput(const QString& placeholder, bool password = false,
                       QWidget* parent = nullptr);

private:
    void paintEvent(QPaintEvent* event) override;
};

#endif  // JUNIORGRAMUI_INPUT_FIELDS_H
