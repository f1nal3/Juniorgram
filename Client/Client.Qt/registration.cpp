#include "registration.hpp"

#include "App.hpp"

Registration::Registration(QWidget* parent) : QWidget(parent)
{
    usernameLineEdit       = std::make_unique<FlatInput>("Username", this);
    passwordLineEdit       = std::make_unique<FlatInput>("Password", true, this);
    passwordRepeatLineEdit = std::make_unique<FlatInput>("Repeat password", true, this);
    registrationButton     = std::make_unique<FlatButton>("Create account", this);
    back                   = std::make_unique<FlatButton>("Back", this);

    logoWidget = std::make_unique<LogoWidget>(this);

    QObject::connect(back.get(), &FlatButton::pressed,
                     [&]() { mainObjectApplication->setAppState(AppS::AppStateS::LoginForm); });

    const int BLOCKWIDTH = Style::valueDPIScale(500);

    usernameLineEdit->resize(BLOCKWIDTH, usernameLineEdit->sizeHint().height());
    usernameLineEdit->show();
    passwordLineEdit->resize(BLOCKWIDTH, passwordLineEdit->sizeHint().height());
    passwordLineEdit->show();
    passwordRepeatLineEdit->resize(BLOCKWIDTH, passwordRepeatLineEdit->sizeHint().height());
    passwordRepeatLineEdit->show();
    registrationButton->resize(BLOCKWIDTH, registrationButton->sizeHint().height());
    registrationButton->show();
    back->resize(BLOCKWIDTH, back->sizeHint().height());
    back->show();
}

void Registration::keyPressEvent(QKeyEvent* event)
{
    if ((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return))
        mainObjectApplication->setAppState(AppS::AppStateS::LoginForm);
}

void Registration::setObjectApplication(Application* objectApplication)
{
    mainObjectApplication = objectApplication;
}

void Registration::resizeEvent(QResizeEvent* event)
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
    passwordRepeatLineEdit->move(LEFT_SHIFT,
                                 passwordLineEdit->geometry().bottom() + 1 + HOR_SPACING);
    registrationButton->move(LEFT_SHIFT,
                             passwordRepeatLineEdit->geometry().bottom() + 1 + HOR_SPACING * 3 / 2);
    back->move(LEFT_SHIFT, registrationButton->geometry().bottom() + 1 + HOR_SPACING);
}
