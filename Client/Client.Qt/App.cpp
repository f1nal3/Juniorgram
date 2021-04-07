#include "App.hpp"

#include <Widgets/BioButton.hpp>

#include "ChatWindow.hpp"
#include "MainWidget.hpp"
#include "login.hpp"
#include "registration.hpp"

namespace App
{
namespace
{
MainWidget* widget;
BioButton* bio_button;
AppState m_app_state = AppState::RegistrationForm;
}  // namespace
void create()
{
    QFontDatabase::addApplicationFont(":fonts/NotoSans-Regular.ttf");
    QFontDatabase::addApplicationFont(":fonts/NotoSans-Bold.ttf");
    QFontDatabase::addApplicationFont(":fonts/NotoSans-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":fonts/NotoSans-Italic.ttf");
    widget = new MainWidget();
    widget->show();
    widget->hide();
#if _WIN32

    HWND handle = reinterpret_cast<HWND>(widget->winId());
    LONG style  = ::GetWindowLong(handle, GWL_STYLE);
    ::SetWindowLong(handle, GWL_STYLE, style | WS_CAPTION | WS_MAXIMIZEBOX | WS_THICKFRAME);
    ::SetWindowPos(handle, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
    // const MARGINS shadow{9, 9, 9, 9};
    //::DwmExtendFrameIntoClientArea(handle, &shadow);
#endif
    setAppState(AppState::LoginForm);
}
void show() { widget->show(); }
void setAppState(AppState app_state)
{
    m_app_state = app_state;
    widget->show();
    if (bio_button)
    {
        delete bio_button;
        bio_button = nullptr;
    }
    switch (m_app_state)
    {
        case AppState::LoginForm:
        {
            auto* wid = new Login();
            widget->setCentralWidget(wid);
        }
        break;
        case AppState::Authorized:
        {
            auto* wid = new ChatWindow();
            bio_button = new BioButton(QImage(), true, widget);
            bio_button->setImage(QImage(":/images/logo.png"));
            widget->refreshTitleBar(bio_button);
            widget->setCentralWidget(wid);
        }
        break;
        case AppState::RegistrationForm:
        {
            auto* wid = new Registration();
            widget->setCentralWidget(wid);
        }
        break;
    }
}
}  // namespace App
