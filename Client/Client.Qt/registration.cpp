#include "registration.h"
#include "login.h"

Registration::Registration(QWidget *parent) : QWidget(parent)
{
    usernameLabel = std::make_unique<QLabel>("&Username");
    passwordLabel = std::make_unique<QLabel>("&Password");
    passwordRepeatLabel = std::make_unique<QLabel>("&Repeat password");
    usernameLineEdit = std::make_unique<QLineEdit>();
    passwordLineEdit = std::make_unique<QLineEdit>();
    passwordRepeatLineEdit = std::make_unique<QLineEdit>();
    registrationButton = std::make_unique<QPushButton>("&Create account");
    usernameLabel->setBuddy(&*usernameLineEdit);
    passwordLabel->setBuddy(&*passwordLineEdit);
    passwordRepeatLabel->setBuddy(&*passwordRepeatLineEdit);

    QObject::connect(&*registrationButton, SIGNAL(clicked()), this, SLOT(hide()));
    QObject::connect(&*registrationButton, SIGNAL(clicked()), this, SLOT(displayLoginWindow()));

    gridLayout = std::make_unique<QGridLayout>();
    gridLayout->addWidget(&*usernameLabel, 0, 0);
    gridLayout->addWidget(&*passwordLabel, 1, 0);
    gridLayout->addWidget(&*passwordRepeatLabel, 2, 0);
    gridLayout->addWidget(&*usernameLineEdit, 0, 1);
    gridLayout->addWidget(&*passwordLineEdit, 1, 1);
    gridLayout->addWidget(&*passwordRepeatLineEdit, 2, 1);
    gridLayout->addWidget(&*registrationButton, 3, 0);
    this->setLayout(&*gridLayout);
}

void Registration::displayLoginWindow()
{
   loginWindow = std::make_unique<Login>();
   loginWindow->show();
}
