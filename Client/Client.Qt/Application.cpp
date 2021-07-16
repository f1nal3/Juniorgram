#include "Application.hpp"

#include "ChatWindow.hpp"
#include "ConnectionManager.hpp"
#include "Style/StyleFont.hpp"
#include "Widgets/BioButton.hpp"
#include "login.hpp"
#include "registration.hpp"

Application::Application(int& argc, char** argv) : QApplication(argc, argv), _bio(nullptr) {}

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
    _bio  = std::make_unique<BioButton>();

    _mainwidget->addWidget(std::make_unique<Login>());
    _mainwidget->addWidget(std::make_unique<Registration>());
    _mainwidget->addWidget(std::make_unique<ChatWindow>());

    ConnectionManager::connect();
    std::thread(&ConnectionManager::loop).detach();
    setAppState(App::AppState::LoginForm);
    QApplication::setFont(st::defaultFont);
}

void Application::show() { _mainwidget->show(); }

void Application::setAppState(App::AppState app_state)
{
    mAppState = app_state;
    if (_bio)
    {
        _bio->hide();
    }
    switch (mAppState)
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
            _bio->setParent(_mainwidget.get());
            _bio->setIcon(_icon);
            _bio->show();
            _mainwidget->refreshTitleBar(_bio.get());
            _mainwidget->setCentralWidget(2);
            break;
        }
    }
}
