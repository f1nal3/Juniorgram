#include "Application.hpp"

#include "ChatWindow.hpp"
#include "ConnectionManager.hpp"
#include "Style/StyleFont.hpp"
#include "Widgets/BioButton.hpp"
#include "login.hpp"
#include "registration.hpp"

Application::Application(int& argc, char** argv) : QApplication(argc, argv) {}

void Application::create()
{
    Style::internal::StartFonts();
    _mainwidget = std::make_unique<MainWidget>();

#if _WIN32
    HWND handle = reinterpret_cast<HWND>(_mainwidget->winId());
    LONG style  = ::GetWindowLong(handle, GWL_STYLE);
    ::SetWindowLong(handle, GWL_STYLE, style | WS_CAPTION | WS_MAXIMIZEBOX | WS_THICKFRAME);
    ::SetWindowPos(handle, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
    // const MARGINS shadow{9, 9, 9, 9};
    //::DwmExtendFrameIntoClientArea(handle, &shadow);
#endif

    _icon = new Style::icon(":/images/logo.png");
    _mainwidget->setBioButtonIcon(_icon);

    ConnectionManager::connect();
    std::thread(&ConnectionManager::loop).detach();

    _mainwidget->addWidget(std::make_unique<Login>());
    _mainwidget->addWidget(std::make_unique<Registration>());

    setAppState(App::AppState::LoginForm);
    QApplication::setFont(st::defaultFont);
}

void Application::show() { _mainwidget->show(); }

void Application::setAppState(App::AppState app_state)
{
    _appState = app_state;
    _mainwidget->refreshTitleBar(false);
    switch (_appState)
    {
        case App::AppState::LoginForm:
        {
            _mainwidget->setCentralWidget(0);
            break;
        }
        case App::AppState::RegistrationForm:
        {
            _mainwidget->setCentralWidget(1);
            break;
        }
        case App::AppState::ChatWindowForm:
        {
            if (_mainwidget->getCountWidgets() < 3)
                _mainwidget->addWidget(std::make_unique<ChatWindow>());
            _mainwidget->refreshTitleBar(true);
            _mainwidget->setCentralWidget(2);
            break;
        }
    }
}
