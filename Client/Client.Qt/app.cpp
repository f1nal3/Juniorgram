#include "app.h"

#include <Widgets/BioButton.h>

#include "MainWidget.h"
#include "login.hpp"
#include "registration.hpp"
#include "ChatWindow.h"
namespace App
{
namespace
{
MainWidget* widget;
BioButton* bio_Button;
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
void show()
{
    widget->show();
    Q_UNUSED(widget);
}
void setAppState(AppState app_state)
{
    m_app_state = app_state;
    widget->show();
    if (bio_Button)
    {
        delete bio_Button;
        bio_Button = nullptr;
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
            auto* wid  = new ChatWindow();
            bio_Button = new BioButton(QImage(), true, widget);
            bio_Button->setImage(QImage(":/images/logo.png"));
            widget->refreshTitleBar(bio_Button);
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
