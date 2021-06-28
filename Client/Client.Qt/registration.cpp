#include "registration.hpp"

Registration::Registration(QWidget* parent) : QWidget(parent)
{
    usernameLineEdit       = std::make_unique<FlatInput>("Username", this);
    passwordLineEdit       = std::make_unique<FlatInput>("Password", true, this);
    passwordRepeatLineEdit = std::make_unique<FlatInput>("Repeat password", true, this);
    registrationButton     = std::make_unique<FlatButton>(this, "Create account");
    back                   = std::make_unique<FlatButton>(this, "Back");

    logoWidget = std::make_unique<LogoWidget>(this);

    back->setClickCallback([]() { oApp->setAppState(App::AppState::LoginForm); });

    const int BLOCKWIDTH = Style::valueDPIScale(500);

    usernameLineEdit->resize(BLOCKWIDTH, usernameLineEdit->sizeHint().height());
    passwordLineEdit->resize(BLOCKWIDTH, passwordLineEdit->sizeHint().height());
    passwordRepeatLineEdit->resize(BLOCKWIDTH, passwordRepeatLineEdit->sizeHint().height());
    registrationButton->resize(BLOCKWIDTH, registrationButton->sizeHint().height());
    back->resize(BLOCKWIDTH, back->sizeHint().height());

    registrationButton->setClickCallback([&]() {
        using namespace UserDataValidation;
        if (passwordLineEdit->text() == passwordRepeatLineEdit->text())
        {
            if (!isLoginValid(usernameLineEdit->text().toStdString()))
            {
                // Say to user that input login is not valid.
                return;
            }
            
            if (!isEmailValid("here must be email"))
            {
                // Say to user that input email is not validt.
                return;
            }

            if (!isPasswordValid(passwordLineEdit->text().toStdString()))
            {
                // Say to user that input password is not valid.
                return;
            }

            ConnectionManager::getClient().userRegistration(usernameLineEdit->text().toStdString(),
                                                            "some_email",
                                                            passwordLineEdit->text().toStdString());
        }
        else
        {
            // Say to user that input passwords are different.
        }
    });
}

void Registration::keyPressEvent(QKeyEvent* event)
{
    if ((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return))
        oApp->setAppState(App::AppState::LoginForm);
}

void Registration::resizeEvent(QResizeEvent* event)
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
    passwordRepeatLineEdit->move(LEFT_SHIFT,
                                 passwordLineEdit->geometry().bottom() + 1 + HOR_SPACING);
    registrationButton->move(LEFT_SHIFT,
                             passwordRepeatLineEdit->geometry().bottom() + 1 + HOR_SPACING * 3 / 2);
    back->move(LEFT_SHIFT, registrationButton->geometry().bottom() + 1 + HOR_SPACING);
}
