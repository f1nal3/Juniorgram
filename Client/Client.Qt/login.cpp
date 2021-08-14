#include "login.hpp"

#include <QtEvents>

#include "Application.hpp"
#include "Utility/UserDataValidation.hpp"
#include "Widgets/Buttons.hpp"
#include "Widgets/InputFields.hpp"

Login::Login(QWidget* parent) : Page(parent)
{
    _usernameInput = std::make_unique<FlatInput>(this, "Username");
    _passwordInput = std::make_unique<FlatInput>(this, "Password", true);

    _signInButton       = std::make_unique<FlatButton>(this, "Login");
    _registrationButton = std::make_unique<FlatButton>(this, "Registration");

    _logoWidget = std::make_unique<LogoWidget>(this);

    connect(ReceiverManager::instance(), &ReceiverManager::onLoginAnswer, this, &Login::onLogin);

    _signInButton->setClickCallback([this]() {
        onPause();
        std::string login    = _usernameInput->text().toStdString();
        std::string password = _passwordInput->text().toStdString();

        ConnectionManager::loginState = LoginState::IN_PROGRESS;
        auto& connectionManager       = oApp->connectionManager();
        connectionManager->userAuthorization(login, password);
    });

    _registrationButton->setClickCallback([=]() { oApp->setAppState(App::AppState::RegistrationForm); });

    _signInButton->resize(st::authBlockWidth, _signInButton->minimumHeight());
    _registrationButton->resize(st::authBlockWidth, _registrationButton->minimumHeight());
    _passwordInput->resize(st::authBlockWidth, _passwordInput->minimumHeight());
    _usernameInput->resize(st::authBlockWidth, _usernameInput->minimumHeight());
}

void Login::resizeEvent(QResizeEvent* event)
{
    const QSize SIZE          = event->size();
    const int   HOR_SPACING   = Style::valueDPIScale(16);
    const int   MIN_TOP_SHIFT = SIZE.height() * 40 / 100;
    const int   LEFT_SHIFT    = (SIZE.width() - st::authBlockWidth) / 2;

    _logoWidget->updateSize();

    _usernameInput->move(LEFT_SHIFT, MIN_TOP_SHIFT);
    _passwordInput->move(LEFT_SHIFT, _usernameInput->geometry().bottom() + 1 + HOR_SPACING);
    _signInButton->move(LEFT_SHIFT, _passwordInput->geometry().bottom() + 1 + HOR_SPACING * 2);
    _registrationButton->move(LEFT_SHIFT, _signInButton->geometry().bottom() + 1 + HOR_SPACING);
    Page::resizeEvent(event);
}

void Login::onLogin()
{
    onResume();
    if (ConnectionManager::loginState == LoginState::SUCCESS)
    {
        _usernameInput->clear();
        _passwordInput->clear();
        oApp->setAppState(App::AppState::ChatWindowForm);
    }
}
