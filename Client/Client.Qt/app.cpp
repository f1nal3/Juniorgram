//
// Created by Stanislav on 22.03.2021.
//

#include "app.h"

#include "login.h"
#include "mainwidget.h"
#include "registration.h"
namespace App
{
namespace
{
MainWidget* widget;
AppState m_app_state;
}  // namespace
void create()
{
    QFontDatabase::addApplicationFont(":fonts/JetBrainsMono-Regular.ttf");
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
void show()
{
    widget->show();
    Q_UNUSED(widget);
}
void setAppState(AppState app_state)
{
    m_app_state = app_state;
    switch (m_app_state)
    {
        case AppState::LoginForm:
        {
            auto* wid = new Login();
            widget->setCentralWidget(wid);
        }
        break;
        case AppState::Authorized:
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