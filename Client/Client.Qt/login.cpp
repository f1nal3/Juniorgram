#include "login.hpp"

Login::Login(QWidget* parent) : QWidget(parent)
{
    passwordLineEdit = new FlatInput("Password", true, this);
    usernameLineEdit = new FlatInput("Username", this);

    buttonSignin       = new FlatButton(this, "Login");
    buttonRegistration = new FlatButton(this, "Registration");

    logoWidget = new LogoWidget(this);

    buttonSignin->setClickCallback([]() { oApp->setAppState(App::AppState::ChatWindowForm); });
    buttonRegistration->setClickCallback(
        []() { oApp->setAppState(App::AppState::RegistrationForm); });

    const int BLOCKWIDTH = Style::valueDPIScale(500);
    buttonSignin->resize(BLOCKWIDTH, buttonSignin->sizeHint().height());
    buttonRegistration->resize(BLOCKWIDTH, buttonRegistration->sizeHint().height());

    passwordLineEdit->resize(BLOCKWIDTH, passwordLineEdit->sizeHint().height());
    usernameLineEdit->resize(BLOCKWIDTH, usernameLineEdit->sizeHint().height());
}

Login::~Login()
{
    delete usernameLineEdit;
    delete passwordLineEdit;
    delete buttonSignin;
    delete buttonRegistration;
    delete logoWidget;
}

void Login::keyPressEvent(QKeyEvent* event)
{
    if ((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return))
        oApp->setAppState(App::AppState::ChatWindowForm);
}

void Login::resizeEvent(QResizeEvent* event)
{
    const QSize SIZE          = event->size();
    const int   HOR_SPACING   = Style::valueDPIScale(16);
    const int   MIN_TOP_SHIFT = SIZE.height() * 40 / 100;
    const int   LEFT_SHIFT    = (SIZE.width() - Style::valueDPIScale(500)) / 2;
    const int   SPACE         = Style::valueDPIScale(10);

    const auto FIT_MAX = logoWidget->bestFit();
    // Aspect ratio;
    const float ASPRAT = FIT_MAX.width() / FIT_MAX.height();

    const auto FIT_WIDTH  = QSize(SIZE.width() - SPACE * 2, (SIZE.width() - SPACE * 2) / ASPRAT);
    const auto FIT_HEIGHT = QSize((MIN_TOP_SHIFT - SPACE * 2) * ASPRAT, MIN_TOP_SHIFT - SPACE * 2);

    auto bestFit = FIT_MAX;
    if (FIT_WIDTH.width() < bestFit.width()) bestFit = FIT_WIDTH;
    if (FIT_HEIGHT.width() < bestFit.width()) bestFit = FIT_HEIGHT;

    logoWidget->resize(bestFit);
    logoWidget->move((width() - bestFit.width()) / 2, (MIN_TOP_SHIFT - bestFit.height()) / 2);

    usernameLineEdit->move(LEFT_SHIFT, MIN_TOP_SHIFT);
    passwordLineEdit->move(LEFT_SHIFT, usernameLineEdit->geometry().bottom() + 1 + HOR_SPACING);
    buttonSignin->move(LEFT_SHIFT, passwordLineEdit->geometry().bottom() + 1 + HOR_SPACING * 2);
    buttonRegistration->move(LEFT_SHIFT, buttonSignin->geometry().bottom() + 1 + HOR_SPACING);
}
