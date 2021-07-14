#include "login.hpp"

#include <QtEvents>

#include "Application.hpp"
#include "Widgets/FlatButton.hpp"
#include "Widgets/InputFields.hpp"
#include "Widgets/LogoWidget.hpp"

Login::Login(QWidget* parent) : QWidget(parent)
{
    passwordLineEdit = std::make_unique<FlatInput>("Password", true, this);
    usernameLineEdit = std::make_unique<FlatInput>("Username", this);

    buttonSignin       = std::make_unique<FlatButton>(this, "Login");
    buttonRegistration = std::make_unique<FlatButton>(this, "Registration");

    logoWidget = std::make_unique<LogoWidget>(this);


    buttonSignin->setClickCallback([]() { oApp->setAppState(App::AppState::ChatWindowForm); });
    buttonRegistration->setClickCallback([]() { oApp->setAppState(App::AppState::RegistrationForm); });


    const int BLOCKWIDTH = Style::valueDPIScale(500);
    buttonSignin->resize(BLOCKWIDTH, buttonSignin->sizeHint().height());
    buttonRegistration->resize(BLOCKWIDTH, buttonRegistration->sizeHint().height());
    passwordLineEdit->resize(BLOCKWIDTH, passwordLineEdit->sizeHint().height());
    usernameLineEdit->resize(BLOCKWIDTH, usernameLineEdit->sizeHint().height());
}

void Login::resizeEvent(QResizeEvent* event)
{
    const QSize SIZE          = event->size();
    const int   HOR_SPACING   = Style::valueDPIScale(16);
    const int   MIN_TOP_SHIFT = SIZE.height() * 40 / 100;
    const int   LEFT_SHIFT    = (SIZE.width() - Style::valueDPIScale(500)) / 2;

    logoWidget->recountSize();

    usernameLineEdit->move(LEFT_SHIFT, MIN_TOP_SHIFT);
    passwordLineEdit->move(LEFT_SHIFT, usernameLineEdit->geometry().bottom() + 1 + HOR_SPACING);
    buttonSignin->move(LEFT_SHIFT, passwordLineEdit->geometry().bottom() + 1 + HOR_SPACING * 2);
    buttonRegistration->move(LEFT_SHIFT, buttonSignin->geometry().bottom() + 1 + HOR_SPACING);
}
