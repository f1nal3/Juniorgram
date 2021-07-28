#include "Application.hpp"

#include "ChatWindow.hpp"
#include "ConnectionManager.hpp"
#include "MainWidget.hpp"
#include "login.hpp"
#include "registration.hpp"

Application::Application(int& argc, char** argv) : QApplication(argc, argv) {}

void Application::create()
{
    Style::internal::StartFonts();
    _mainWidget = std::make_unique<MainWidget>();

#if _WIN32
    HWND handle = reinterpret_cast<HWND>(_mainWidget->winId());
    LONG style  = ::GetWindowLong(handle, GWL_STYLE);
    ::SetWindowLong(handle, GWL_STYLE, style | WS_CAPTION | WS_MAXIMIZEBOX | WS_THICKFRAME);
    ::SetWindowPos(handle, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
    // const MARGINS shadow{9, 9, 9, 9};
    //::DwmExtendFrameIntoClientArea(handle, &shadow);
#endif

    _icon = new Style::icon(":/images/logo.png");
    _mainWidget->setBioButtonIcon(_icon);

    _mainWidget->addWidget(std::make_unique<Login>());
    _mainWidget->addWidget(std::make_unique<Registration>());
    _mainWidget->addWidget(std::make_unique<ChatWindow>());
  
    ConnectionManager::connect();
    std::thread(&ConnectionManager::loop).detach();

    setAppState(App::AppState::LoginForm);
    QApplication::setFont(st::defaultFont);
}

void Application::show() { _mainWidget->show(); }

void Application::setAppState(App::AppState app_state)
{
    _appState = app_state;
    _mainWidget->refreshTitleBar(false);
    switch (_appState)
    {
        case App::AppState::LoginForm:
        {
            _mainWidget->setCentralWidget(0);
            break;
        }
        case App::AppState::RegistrationForm:
        {
            _mainWidget->setCentralWidget(1);
            break;
        }
        case App::AppState::ChatWindowForm:
        {
            _mainWidget->refreshTitleBar(true);
            _mainWidget->setCentralWidget(2);
            break;
        }
    }
}
