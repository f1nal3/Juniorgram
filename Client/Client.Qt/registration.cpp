#include "registration.hpp"

#include <QtEvents>
#include <iostream>

#include "ConnectionManager.hpp"
#include "Utility/UserDataValidation.hpp"

Registration::Registration(QWidget* parent) : QWidget(parent)
{
    _emailInput            = std::make_unique<FlatInput>(this, "Email");
    _usernameInput         = std::make_unique<FlatInput>(this, "Username");
    _passwordInput         = std::make_unique<FlatInput>(this, "Password", true);
    _repeatPasswordInput   = std::make_unique<FlatInput>(this, "Repeat password", true);
    _registrationButton    = std::make_unique<FlatButton>(this, "Create account");
    _backButton            = std::make_unique<FlatButton>(this, "Back");

    _logoWidget = std::make_unique<LogoWidget>(this);

    _backButton->setClickCallback([]() { oApp->setAppState(App::AppState::LoginForm); });

    const int BLOCKWIDTH = Style::valueDPIScale(500);

    _emailInput->resize(BLOCKWIDTH, _emailInput->sizeHint().height());
    _usernameInput->resize(BLOCKWIDTH, _usernameInput->sizeHint().height());
    _passwordInput->resize(BLOCKWIDTH, _passwordInput->sizeHint().height());
    _repeatPasswordInput->resize(BLOCKWIDTH, _repeatPasswordInput->sizeHint().height());
    _registrationButton->resize(BLOCKWIDTH, _registrationButton->sizeHint().height());
    _backButton->resize(BLOCKWIDTH, _backButton->sizeHint().height());
    _logoWidget->setPart(30);

    _registrationButton->setClickCallback([this]() {
        using namespace UserDataValidation;

        std::string email          = _emailInput->text().toStdString();
        std::string login          = _usernameInput->text().toStdString();
        std::string password       = _passwordInput->text().toStdString();
        std::string repeatPassword = _repeatPasswordInput->text().toStdString();

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

        oApp->setAppState(App::AppState::LoginForm);
    });
}

void Registration::resizeEvent(QResizeEvent* event)
{
    const QSize SIZE          = event->size();
    const int   HOR_SPACING   = Style::valueDPIScale(16);
    const int   MIN_TOP_SHIFT = SIZE.height() * 30 / 100;
    const int   LEFT_SHIFT    = (SIZE.width() - Style::valueDPIScale(500)) / 2;
    _logoWidget->recountSize();

    _emailInput->move(LEFT_SHIFT, MIN_TOP_SHIFT);
    _usernameInput->move(LEFT_SHIFT, _emailInput->geometry().bottom() + 1 + HOR_SPACING);
    _passwordInput->move(LEFT_SHIFT, _usernameInput->geometry().bottom() + 1 + HOR_SPACING);
    _repeatPasswordInput->move(LEFT_SHIFT, _passwordInput->geometry().bottom() + 1 + HOR_SPACING);
    _registrationButton->move(LEFT_SHIFT, _repeatPasswordInput->geometry().bottom() + 1 + HOR_SPACING * 3 / 2);
    _backButton->move(LEFT_SHIFT, _registrationButton->geometry().bottom() + 1 + HOR_SPACING);
}
