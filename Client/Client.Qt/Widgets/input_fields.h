//
// Created by Stanislav on 11.03.2021.
//

#ifndef JUNIORGRAMUI_INPUT_FIELDS_H
#define JUNIORGRAMUI_INPUT_FIELDS_H

#include "../Style/style.h"
#include "../pch.h"

class FlatInput : public QLineEdit
{
    Q_OBJECT
public:
    explicit FlatInput(QWidget* parent = nullptr);
};

#endif  // JUNIORGRAMUI_INPUT_FIELDS_H
