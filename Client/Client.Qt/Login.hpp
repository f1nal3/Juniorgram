#pragma once
#include "Widgets/FlatButton.hpp"
#include "Widgets/InputFields.hpp"
#include "pch.hpp"

class Login : public QWidget
{
    Q_OBJECT
public:
    explicit Login(QWidget* parent = nullptr);

private:
    std::unique_ptr<QGridLayout> gridLayout;
    std::unique_ptr<FlatInput> usernameLineEdit;
    std::unique_ptr<FlatInput> passwordLineEdit;
    std::unique_ptr<QPushButton> buttonSignin;
    std::unique_ptr<QPushButton> buttonRegistration;
    void keyPressEvent(QKeyEvent* event);
};
