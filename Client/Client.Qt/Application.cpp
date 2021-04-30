#include "Application.hpp"

Application::Application(int& argc, char** argv) : QApplication(argc, argv)
{
    mBioButton = nullptr;
}

void Application::create()
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
    
    ConnectionManager::connect();
    std::thread(&ConnectionManager::loop).detach();

    setAppState(AppState::LoginForm);
    auto font = QFont("Noto Sans", 12);
    font.setPixelSize(Style::valueDPIScale(15));
    QApplication::setFont(font);
}

void Application::show() { mMainWidget->show(); }

void Application::setAppState(AppState app_state)
{
    mAppState = app_state;
    mMainWidget->show();
    if (mBioButton != nullptr)
    {
        delete mBioButton;
        mBioButton = nullptr;
    }
    switch (mAppState)
    {
        case AppState::LoginForm:
        {
            auto* wid  = new Login();
            mBioButton = nullptr;
            mMainWidget->setCentralWidget(wid);
            break;
        }
        case AppState::RegistrationForm:
        {
            auto* wid = new Registration();
            mBioButton = nullptr;
            mMainWidget->setCentralWidget(wid);
            break;
        }
        case AppState::AuthorizedForm:
        {
            auto* wid  = new ChatWindow();
            mBioButton = new BioButton(QImage(), true, mMainWidget);
            mBioButton->setImage(QImage(":/images/logo.png"));
            mMainWidget->refreshTitleBar(mBioButton);
            mMainWidget->setCentralWidget(wid);
            break;
        }
        case AppState::ChatWindowForm:
        {
            auto* wid  = new ChatWindow();
            mBioButton = new BioButton(QImage(), true, mMainWidget);
            mMainWidget->refreshTitleBar(mBioButton);
            mMainWidget->setCentralWidget(wid);
            break;
        }
    }
}
