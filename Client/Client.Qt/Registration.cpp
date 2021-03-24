#include "Registration.h"

#include "app.h"

Registration::Registration(QWidget* parent) : QWidget(parent)
{
    usernameLineEdit       = std::make_unique<FlatInput>("Username");
    passwordLineEdit       = std::make_unique<FlatInput>("Password", true);
    passwordRepeatLineEdit = std::make_unique<FlatInput>("Repeat password", true);
    registrationButton     = std::make_unique<FlatButton>("Create account");
    back                   = std::make_unique<FlatButton>("Back");

    QObject::connect(registrationButton.get(), &FlatButton::pressed, []() {
        QDesktopServices::openUrl(QUrl("https://www.youtube.com/watch?v=dQw4w9WgXcQ"));
    });

    QObject::connect(back.get(), &FlatButton::pressed,
                     []() { App::setAppState(AppState::LoginForm); });

    gridLayout = std::make_unique<QGridLayout>();
    gridLayout->addWidget(usernameLineEdit.get(), 0, 0);
    gridLayout->addWidget(passwordLineEdit.get(), 1, 0);
    gridLayout->addWidget(passwordRepeatLineEdit.get(), 2, 0);
    gridLayout->addWidget(registrationButton.get(), 3, 0);
    gridLayout->addWidget(back.get(), 4, 0);
    this->setLayout(gridLayout.get());
}
