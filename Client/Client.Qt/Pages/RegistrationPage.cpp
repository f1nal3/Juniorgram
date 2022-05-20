#include "RegistrationPage.hpp"

#include <QtEvents>
#include <iostream>

#include "Application.hpp"
#include "Utility/UserDataValidation.hpp"
#include "Widgets/Buttons.hpp"
#include "Widgets/InputFields.hpp"
#include "Widgets/LogoWidget.hpp"

RegistrationPage::RegistrationPage(QWidget* parent) : Page(parent)
{
    _emailInput          = std::make_unique<FlatInput>(this, "Email");
    _usernameInput       = std::make_unique<FlatInput>(this, "Username");
    _passwordInput       = std::make_unique<FlatInput>(this, "Password", true);
    _repeatPasswordInput = std::make_unique<FlatInput>(this, "Repeat password", true);
    _registrationButton  = std::make_unique<FlatButton>(this, "Create account");
    _backButton          = std::make_unique<FlatButton>(this, "Back");

    _logoWidget = std::make_unique<LogoWidget>(this);
    _popupSystemMessage = std::make_unique<PopupSystemMessage>(this);

    _registrationHotkey = std::make_unique<QShortcut>
    (
        QKeySequence{ Qt::Key_Return },
        this,
        [this]() { _registrationButton->clicked(Qt::NoModifier, Qt::LeftButton); }
    );

    _backButton->setClickCallback([]() { oApp->setAppState(App::AppState::LoginForm); });

    _emailInput->resize(st::authBlockWidth, _emailInput->minimumHeight());
    _usernameInput->resize(st::authBlockWidth, _usernameInput->minimumHeight());
    _passwordInput->resize(st::authBlockWidth, _passwordInput->minimumHeight());
    _repeatPasswordInput->resize(st::authBlockWidth, _repeatPasswordInput->minimumHeight());
    _registrationButton->resize(st::authBlockWidth, _registrationButton->minimumHeight());
    _backButton->resize(st::authBlockWidth, _backButton->minimumHeight());
    _logoWidget->setPart(30);

    connect(ReceiverManager::instance(), &ReceiverManager::onRegistrationAnswer, this, &RegistrationPage::onRegistration);

    _registrationButton->setClickCallback([this]() {
        using namespace UserDataValidation;
        onPause();
        std::string email          = _emailInput->text().toStdString();
        std::string login          = _usernameInput->text().toStdString();
        std::string password       = _passwordInput->text().toStdString();
        std::string repeatPassword = _repeatPasswordInput->text().toStdString();

        if (email.empty() || login.empty() || password.empty() || repeatPassword.empty())
        {
            Base::Logger::FileLogger::getInstance().log("some field is empty", Base::Logger::LogLevel::WARNING);
            messageOut(_popupWidget, "some field is empty");
            onResume();

            return;
        }

        if (password != repeatPassword)
        {
            Base::Logger::FileLogger::getInstance().log("passwords are different", Base::Logger::LogLevel::WARNING);
            messageOut(_popupWidget, "passwords are different");
            onResume();

            return;
        }

        if (!isLoginValid(login))
        {
            Base::Logger::FileLogger::getInstance().log("login is not valid", Base::Logger::LogLevel::WARNING);
            messageOut(_popupWidget, "login is not valid");
            onResume();

            return;
        }

        if (!isEmailValid(email))
        {
            Base::Logger::FileLogger::getInstance().log("email is not valid", Base::Logger::LogLevel::WARNING);
            messageOut(_popupWidget, "email is not valid");
            onResume();
        {
            Base::Logger::FileLogger::getInstance().log("password is not valid", Base::Logger::LogLevel::WARNING);
            messageOut(_popupWidget, "password is not valid");
            onResume();

            return;
        }
        oApp->connectionManager()->userRegistration(email, login, password);
    });
}

void RegistrationPage::resizeEvent(QResizeEvent* event)
{
    const QSize SIZE          = event->size();
    const int   MIN_TOP_SHIFT = SIZE.height() * 30 / 100;
    const int   LEFT_SHIFT    = (SIZE.width() - st::authBlockWidth) / 2;
    _logoWidget->updateSize();

    _emailInput->move(LEFT_SHIFT, MIN_TOP_SHIFT);
    _usernameInput->move(LEFT_SHIFT, _emailInput->geometry().bottom() + 1 + st::authBlockSpacing);
    _passwordInput->move(LEFT_SHIFT, _usernameInput->geometry().bottom() + 1 + st::authBlockSpacing);
    _repeatPasswordInput->move(LEFT_SHIFT, _passwordInput->geometry().bottom() + 1 + st::authBlockSpacing);
    _registrationButton->move(LEFT_SHIFT, _repeatPasswordInput->geometry().bottom() + 1 + st::authBlockSpacing * 3 / 2);
    _backButton->move(LEFT_SHIFT, _registrationButton->geometry().bottom() + 1 + st::authBlockSpacing);
    Page::resizeEvent(event);
}

void RegistrationPage::onRegistration(Utility::RegistrationCodes code)
{
    onResume();
    if (code == Utility::RegistrationCodes::SUCCESS)
    {
        _emailInput->clear();
        _usernameInput->clear();
        _passwordInput->clear();
        _repeatPasswordInput->clear();
        oApp->setAppState(App::AppState::LoginForm);
    }
    else
    {
        // T\todo notification

        if (code == Utility::RegistrationCodes::EMAIL_ALREADY_EXISTS)
        {
            Base::Logger::FileLogger::getInstance().log("Email already exists", Base::Logger::LogLevel::WARNING);
            messageOut(_popupWidget, "Email already exists");
            onResume();
        }
        else if (code == Utility::RegistrationCodes::LOGIN_ALREADY_EXISTS)
        {
            Base::Logger::FileLogger::getInstance().log("Username already taken", Base::Logger::LogLevel::WARNING);
            messageOut(_popupWidget, "Username already taken");
            onResume();
        }
        else
        {
            Base::Logger::FileLogger::getInstance().log("Unknown code", Base::Logger::LogLevel::WARNING);
            messageOut(_popupWidget, "Unknown code");
            onResume();
        }
    }
}

