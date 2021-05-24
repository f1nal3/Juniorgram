#include "Application.hpp"

#include "Style/Style_font.hpp"
#include "login.hpp"
#include "registration.hpp"

Application::Application(int& argc, char** argv) : QApplication(argc, argv), mBioButton(nullptr) {}

void Application::create()
{
    Style::internal::StartFonts();
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

    ConnectionManager::connect();
    std::thread(&ConnectionManager::loop).detach();

    setAppState(App::AppState::LoginForm);
    auto font = QFont("Noto Sans", 12);
    font.setPixelSize(Style::valueDPIScale(15));
    font.setStyleStrategy(QFont::PreferQuality);
    QApplication::setFont(font);
}

void Application::show() { mMainWidget->show(); }

void Application::setAppState(App::AppState app_state)
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
        case App::AppState::LoginForm:
        {
            auto* wid = new Login();
            mMainWidget->setCentralWidget(wid);
            break;
        }
        case App::AppState::RegistrationForm:
        {
            auto* wid = new Registration();
            mMainWidget->setCentralWidget(wid);
            break;
        }
        case App::AppState::ChatWindowForm:
        {
            auto* wid  = new ChatWindow();
            mBioButton = new BioButton(QImage(), true, mMainWidget);
            mBioButton->setImage(QImage(":/images/logo.png"));
            mMainWidget->refreshTitleBar(mBioButton);
            mMainWidget->setCentralWidget(wid);
            break;
        }
    }
}
