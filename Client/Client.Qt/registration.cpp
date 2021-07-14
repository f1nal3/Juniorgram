#include "registration.hpp"

#include <QtEvents>
#include <iostream>

#include "ConnectionManager.hpp"
#include "Utility/UserDataValidation.hpp"

Registration::Registration(QWidget* parent) : QWidget(parent)
{
    emailLineEdit          = std::make_unique<FlatInput>("Email", this);
    usernameLineEdit       = std::make_unique<FlatInput>("Username", this);
    passwordLineEdit       = std::make_unique<FlatInput>("Password", this);
    passwordRepeatLineEdit = std::make_unique<FlatInput>("Repeat password", this);
    registrationButton     = std::make_unique<FlatButton>(this, "Create account");
    back                   = std::make_unique<FlatButton>(this, "Back");

    logoWidget = std::make_unique<LogoWidget>(this);

    back->setClickCallback([]() { oApp->setAppState(App::AppState::LoginForm); });

    const int BLOCKWIDTH = Style::valueDPIScale(500);

    emailLineEdit->resize(BLOCKWIDTH, emailLineEdit->sizeHint().height());
    usernameLineEdit->resize(BLOCKWIDTH, usernameLineEdit->sizeHint().height());
    passwordLineEdit->resize(BLOCKWIDTH, passwordLineEdit->sizeHint().height());
    passwordRepeatLineEdit->resize(BLOCKWIDTH, passwordRepeatLineEdit->sizeHint().height());
    registrationButton->resize(BLOCKWIDTH, registrationButton->sizeHint().height());
    back->resize(BLOCKWIDTH, back->sizeHint().height());
    logoWidget->setPart(30);

    registrationButton->setClickCallback([this]() {
        using namespace UserDataValidation;

        std::string email          = emailLineEdit->text().toStdString();
        std::string login          = usernameLineEdit->text().toStdString();
        std::string password       = passwordLineEdit->text().toStdString();
        std::string repeatPassword = passwordRepeatLineEdit->text().toStdString();

        if (email.empty() || login.empty() || password.empty() || repeatPassword.empty())
        {
            std::cout << "some field is empty" << std::endl;
            return;
        }

        if (password != repeatPassword)
        {
            std::cout << "passwords are different" << std::endl;
            return;
        }

        if (!isLoginValid(login))
        {
            std::cout << "login is not valid" << std::endl;
            return;
        }

        if (!isEmailValid(email))
        {
            std::cout << "email is not valid" << std::endl;
            return;
        }

        if (!isPasswordValid(password))
        {
            std::cout << "password is not valid" << std::endl;
            return;
        }

        ConnectionManager::getClient().userRegistration(email, login, password);
    });
}

void Registration::resizeEvent(QResizeEvent* event)
{
    const QSize SIZE          = event->size();
    const int   HOR_SPACING   = Style::valueDPIScale(16);
    const int   MIN_TOP_SHIFT = SIZE.height() * 30 / 100;
    const int   LEFT_SHIFT    = (SIZE.width() - Style::valueDPIScale(500)) / 2;
    logoWidget->recountSize();

    emailLineEdit->move(LEFT_SHIFT, MIN_TOP_SHIFT);
    usernameLineEdit->move(LEFT_SHIFT, emailLineEdit->geometry().bottom() + 1 + HOR_SPACING);
    passwordLineEdit->move(LEFT_SHIFT, usernameLineEdit->geometry().bottom() + 1 + HOR_SPACING);
    passwordRepeatLineEdit->move(LEFT_SHIFT, passwordLineEdit->geometry().bottom() + 1 + HOR_SPACING);
    registrationButton->move(LEFT_SHIFT, passwordRepeatLineEdit->geometry().bottom() + 1 + HOR_SPACING * 3 / 2);
    back->move(LEFT_SHIFT, registrationButton->geometry().bottom() + 1 + HOR_SPACING);
}
