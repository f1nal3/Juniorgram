#include "login.hpp"
#include "Utility/UserDataValidation.hpp"

#include <QtEvents>

#include "Application.hpp"
#include "ConnectionManager.hpp"
#include "Widgets/FlatButton.hpp"
#include "Widgets/InputFields.hpp"
#include "Widgets/LogoWidget.hpp"

Login::Login(QWidget* parent) : QWidget(parent)
{
    _usernameInput   = std::make_unique<FlatInput>(this, "Username");
    _passwordInput   = std::make_unique<FlatInput>(this, "Password", true);

    _signInButton      = std::make_unique<FlatButton>(this, "Login");
    _registrationButton = std::make_unique<FlatButton>(this, "Registration");

    _logoWidget = std::make_unique<LogoWidget>(this);

    _registrationButton->setClickCallback([]() { oApp->setAppState(App::AppState::RegistrationForm); });
    _signInButton->setClickCallback([this]() {        
        std::string login = _usernameInput->text().toStdString();
        std::string password = _passwordInput->text().toStdString();
        std::cout << "DEBUG: Login: " << login << " pwd: " << password << '\n';
        ConnectionManager::getClient().userAuthorization(login, password);
        });

    _registrationButton->setClickCallback(
        []() { oApp->setAppState(App::AppState::RegistrationForm); });

    const int BLOCKWIDTH = Style::valueDPIScale(500);
    _signInButton->resize(BLOCKWIDTH, _signInButton->sizeHint().height());
    _registrationButton->resize(BLOCKWIDTH, _registrationButton->sizeHint().height());
    _passwordInput->resize(BLOCKWIDTH, _passwordInput->sizeHint().height());
    _usernameInput->resize(BLOCKWIDTH, _usernameInput->sizeHint().height());
}

void Login::resizeEvent(QResizeEvent* event)
{
    const QSize SIZE          = event->size();
    const int   HOR_SPACING   = Style::valueDPIScale(16);
    const int   MIN_TOP_SHIFT = SIZE.height() * 40 / 100;
    const int   LEFT_SHIFT    = (SIZE.width() - Style::valueDPIScale(500)) / 2;

    _logoWidget->recountSize();

    _usernameInput->move(LEFT_SHIFT, MIN_TOP_SHIFT);
    _passwordInput->move(LEFT_SHIFT, _usernameInput->geometry().bottom() + 1 + HOR_SPACING);
    _signInButton->move(LEFT_SHIFT, _passwordInput->geometry().bottom() + 1 + HOR_SPACING * 2);
    _registrationButton->move(LEFT_SHIFT, _signInButton->geometry().bottom() + 1 + HOR_SPACING);
}
