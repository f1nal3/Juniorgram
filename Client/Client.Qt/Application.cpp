#include "Application.hpp"

#include <QSystemTrayIcon>

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
    _mainWidget     = std::make_unique<MainWidget>();
    _systemTrayIcon = std::make_unique<QSystemTrayIcon>(this);
    _systemTrayIcon->setIcon(QIcon(":/images/logo.png"));
    _systemTrayIcon->show();
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

    _repContainer    = std::make_unique<DataAccess::LiteRepositoryContainer>(DataAccess::LiteAdapter::Instance("client.db"));
    _receiverManager = std::make_unique<ReceiverManager>();
    _repContainer->registerRepository<DataAccess::IMessageRepository, DataAccess::LiteMessageRepository>();
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

    // T\todo it now may crash
    //  connect(ReceiverManager::instance(), &ReceiverManager::onDisconnect, this, &Application::reconnectToServer);

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

std::unique_ptr<DataAccess::LiteRepositoryContainer>& Application::repositoryContainer() { return _repContainer; }

void Application::reconnectToServer() { _connectionManager->init(); }

void Application::showMessage(const QString& header, const QString& body, MessageType type, int msecs)
{
    auto messageIcon = QSystemTrayIcon::MessageIcon::Information;
    if (type == MessageType::Warning) messageIcon = QSystemTrayIcon::MessageIcon::Warning;
    if (type == MessageType::Error) messageIcon = QSystemTrayIcon::MessageIcon::Critical;
    _systemTrayIcon->showMessage(header, body, messageIcon, msecs);
}
