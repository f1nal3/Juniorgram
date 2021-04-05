#include "login.hpp"

#include "app.h"

Login::Login(QWidget* parent) : QWidget(parent)
{
    passwordLineEdit = std::make_unique<FlatInput>("Password", true);
    usernameLineEdit = std::make_unique<FlatInput>("Username");

    buttonSignin       = std::make_unique<FlatButton>("Login");
    buttonRegistration = std::make_unique<FlatButton>("Registration");

    QObject::connect(buttonSignin.get(), &FlatButton::pressed,
                     []() { App::setAppState(AppState::Authorized); });
    QObject::connect(buttonRegistration.get(), &FlatButton::pressed,
                     []() { App::setAppState(AppState::RegistrationForm); });

    gridLayout = std::make_unique<QGridLayout>();
    gridLayout->addWidget(usernameLineEdit.get(), 0, 0);
    gridLayout->addWidget(passwordLineEdit.get(), 1, 0);
    gridLayout->addWidget(buttonSignin.get(), 2, 0);
    gridLayout->addWidget(buttonRegistration.get(), 3, 0);
    this->setLayout(gridLayout.get());
}