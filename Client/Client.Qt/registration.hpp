#pragma once
#include "Widgets/FlatButton.h"
#include "Widgets/InputFields.h"
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
    void keyPressEvent(QKeyEvent* event);
};
