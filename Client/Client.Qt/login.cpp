#include "login.h"
#include "registration.h"
#include "mainWindow.h"

Login::Login(QWidget *parent) : QWidget(parent)
{
    passwordLineEdit = std::make_unique<QLineEdit>();
    usernameLineEdit = std::make_unique<QLineEdit>();
    usernameLabel = std::make_unique<QLabel>("&Username:");
    passwordLabel = std::make_unique<QLabel>("&Password:");
    usernameLabel->setBuddy(&*usernameLineEdit);
    passwordLabel->setBuddy(&*passwordLineEdit);

    buttonSignin = std::make_unique<QPushButton>("&Signin");
    buttonRegistration = std::make_unique<QPushButton>("&Registration");

    QObject::connect(&*buttonSignin, SIGNAL(clicked()), this, SLOT(hide()));
    QObject::connect(&*buttonSignin, SIGNAL(clicked()), this, SLOT(displayMainWindow()));
    QObject::connect(&*buttonRegistration, SIGNAL(clicked()), this, SLOT(hide()));
    QObject::connect(&*buttonRegistration, SIGNAL(clicked()), this, SLOT(displayRegistrationWindow()));

    gridLayout = std::make_unique<QGridLayout>();
    gridLayout->addWidget(&*usernameLabel, 0, 0);
    gridLayout->addWidget(&*usernameLineEdit, 0, 1);
    gridLayout->addWidget(&*passwordLabel, 1, 0);
    gridLayout->addWidget(&*passwordLineEdit, 1, 1);
    gridLayout->addWidget(&*buttonSignin, 2, 0);
    gridLayout->addWidget(&*buttonRegistration, 3, 0);
    this->setLayout(&*gridLayout);
}

void Login::displayRegistrationWindow()
{
    registrationWindow = std::make_unique<Registration>();
    registrationWindow->show();
}

void Login::displayMainWindow()
{
    mainWindow = std::make_unique<MainWindow>();
    mainWindow->show();
}
