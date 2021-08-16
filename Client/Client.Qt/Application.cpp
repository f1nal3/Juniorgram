#include "Application.hpp"

#include <Style/Styles.hpp>

#include "MainWidget.hpp"
#include "Pages/ChatPage.hpp"
#include "Pages/LoginPage.hpp"
#include "Pages/RegistrationPage.hpp"

Application::Application(int& argc, char** argv) : QApplication(argc, argv) {}

void Application::create()
{
    setOrganizationName("L&D C++ Lab");
    setApplicationName("Juniorgram");

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

    _receiverManager = std::make_unique<ReceiverManager>();
    _mainWidget->addWidget(std::make_unique<LoginPage>());
    _mainWidget->addWidget(std::make_unique<RegistrationPage>());
    _mainWidget->addWidget(std::make_unique<ChatPage>());

    ReactionLayout::addIcon(0, st::likeIcon);
    ReactionLayout::addIcon(1, st::dislikeIcon);
    ReactionLayout::addIcon(2, st::fireIcon);
    ReactionLayout::addIcon(3, st::catIcon);
    ReactionLayout::addIcon(4, st::smileIcon);

    _connectionManager = std::make_unique<ConnectionManager>();
    _connectionManager->init();

    connect(ReceiverManager::instance(), &ReceiverManager::onDisconnect, this, &Application::reconnectToServer);

    setAppState(App::AppState::LoginForm);
    QApplication::setFont(st::defaultFont);
}

void Application::show() { _mainWidget->show(); }

void Application::setAppState(const App::AppState appState)
{
    _appState = appState;
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

std::unique_ptr<ConnectionManager>& Application::connectionManager() { return _connectionManager; }

void Application::reconnectToServer() { _connectionManager->init(); }
