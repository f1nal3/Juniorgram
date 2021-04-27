#include "login.hpp"

#include "App.hpp"

Login::Login(QWidget* parent) : QWidget(parent)
{
    passwordLineEdit = std::make_unique<FlatInput>("Password", true, this);
    usernameLineEdit = std::make_unique<FlatInput>("Username", this);

    buttonSignin       = std::make_unique<FlatButton>("Login", this);
    buttonRegistration = std::make_unique<FlatButton>("Registration", this);

    logoWidget = std::make_unique<LogoWidget>(this);

    QObject::connect(buttonSignin.get(), &FlatButton::pressed,
                     []() { App::setAppState(AppState::Authorized); });
    QObject::connect(buttonRegistration.get(), &FlatButton::pressed,
                     []() { App::setAppState(AppState::RegistrationForm); });


    const int BLOCKWIDTH = Style::valueDPIScale(500);
    buttonSignin->resize(BLOCKWIDTH, buttonSignin->sizeHint().height());
    buttonSignin->show();
    buttonRegistration->resize(BLOCKWIDTH, buttonRegistration->sizeHint().height());
    buttonRegistration->show();

    passwordLineEdit->resize(BLOCKWIDTH, passwordLineEdit->sizeHint().height());
    passwordLineEdit->show();
    usernameLineEdit->resize(BLOCKWIDTH, usernameLineEdit->sizeHint().height());
    usernameLineEdit->show();
}

void Login::keyPressEvent(QKeyEvent* event)
{
    if ((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return))
        App::setAppState(AppState::Authorized);
}
void Login::resizeEvent(QResizeEvent* event)
{
    const QSize SIZE        = event->size();
    const int HOR_SPACING   = Style::valueDPIScale(16);
    const int MIN_TOP_SHIFT = SIZE.height() * 40 / 100;
    const int LEFT_SHIFT    = (SIZE.width() - Style::valueDPIScale(500)) / 2;
    const int SPACE         = Style::valueDPIScale(10);

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
