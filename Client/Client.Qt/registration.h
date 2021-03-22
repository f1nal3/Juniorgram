#pragma once

#include <QPushButton>

#include "Widgets/input_fields.h"
#include "Widgets/flat_button.h"
#include "pch.h"

class Registration : public QWidget
{
    Q_OBJECT
public:
    explicit Registration(QWidget* parent = nullptr);

private:
    std::unique_ptr<FlatInput> usernameLineEdit;
    std::unique_ptr<FlatInput> passwordLineEdit;
    std::unique_ptr<FlatInput> passwordRepeatLineEdit;
    std::unique_ptr<FlatButton> registrationButton;
    std::unique_ptr<FlatButton> back;
    std::unique_ptr<QGridLayout> gridLayout;
};
