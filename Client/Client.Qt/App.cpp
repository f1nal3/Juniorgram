
#include <Widgets/BioButton.hpp>

#include "App.hpp"
#include "ChatWindow.hpp"
#include "MainWidget.hpp"
#include "Login.hpp"
#include "Registration.hpp"
#include <Client.hpp>

namespace App
{
namespace
{
MainWidget* mMainWidget;
BioButton* mBioButton;
AppState mAppState = AppState::RegistrationForm;
Network::Client client;
}  // namespace
void create()
{
    QFontDatabase::addApplicationFont(":fonts/NotoSans-Regular.ttf");
    QFontDatabase::addApplicationFont(":fonts/NotoSans-Bold.ttf");
    QFontDatabase::addApplicationFont(":fonts/NotoSans-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":fonts/NotoSans-Italic.ttf");
    mMainWidget = new MainWidget();
    mMainWidget->show();
    mMainWidget->hide();
#if _WIN32

    HWND handle = reinterpret_cast<HWND>(mMainWidget->winId());
    LONG style  = ::GetWindowLong(handle, GWL_STYLE);
    ::SetWindowLong(handle, GWL_STYLE, style | WS_CAPTION | WS_MAXIMIZEBOX | WS_THICKFRAME);
    ::SetWindowPos(handle, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
    // const MARGINS shadow{9, 9, 9, 9};
    //::DwmExtendFrameIntoClientArea(handle, &shadow);
#endif
    setAppState(AppState::LoginForm);
}
void show() { mMainWidget->show(); }
void setAppState(AppState app_state)
{
    mAppState = app_state;
    mMainWidget->show();
    if (mBioButton)
    {
        delete mBioButton;
        mBioButton = nullptr;
    }
    switch (mAppState)
    {
        case AppState::ConnectToServer:
        {
            const std::string address = "104.40.239.183";
            const std::uint16_t port  = 65001;

            client.connect(address, port);
        }
        break;
        case AppState::LoginForm:
        {
            auto* wid = new Login();
            mMainWidget->setCentralWidget(wid);
        }
        break;
        case AppState::Authorized:
        {
            auto* wid = new ChatWindow();
            mBioButton = new BioButton(QImage(), true, mMainWidget);
            mBioButton->setImage(QImage(":/images/logo.png"));
            mMainWidget->refreshTitleBar(mBioButton);
            mMainWidget->setCentralWidget(wid);
        }
        break;
        case AppState::RegistrationForm:
        {
            auto* wid = new Registration();
            mMainWidget->setCentralWidget(wid);
        }
        break;
    }
}
}  // namespace App
