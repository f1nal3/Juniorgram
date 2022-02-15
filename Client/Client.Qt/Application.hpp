#pragma once
#include <DataAccess.SQLite/LiteRepositoryContainer.hpp>
#include <QApplication>
#include <Style/Styles.hpp>
#include <memory>

#include "ConnectionManager.hpp"

class MainWidget;
class QSystemTrayIcon;

#define oApp (static_cast<Application*>(QCoreApplication::instance()))

namespace App
{
/**
 * @enum AppState
 * @brief Current app state
 */
enum class AppState
{
    LoginForm,
    RegistrationForm,
    ChatWindowForm
};
}  // namespace App

/// Notification message type
enum class MessageType
{
    Info,
    Error,
    Warning
};

/**
 * @class Application
 * @brief Application abstraction
 */
class Application : public QApplication
{
public:
    /**
     * @brief Application constructor
     * @param argc Arguments count
     * @param argv Arguments
     */
    Application(int& argc, char** argv);
    /// Creates window and initialize all stuff
    void create();
    /// Show the window
    void show();

    /// Connection manager
    std::unique_ptr<ConnectionManager>& connectionManager();
    /// Repository container
    std::unique_ptr<DataAccess::LiteRepositoryContainer>& repositoryContainer();
    /// Shows message as notification
    void showMessage(const QString& header, const QString& body, MessageType type = MessageType::Info, int msecs = 5000);

    /**
     * @brief Change app state
     * @param appState App state
     */
    void setAppState(App::AppState appState);
public slots:
    /// Reconnects to server
    void reconnectToServer();

private:
    std::unique_ptr<DataAccess::LiteRepositoryContainer> _repContainer;

    std::unique_ptr<MainWidget>        _mainWidget;
    std::unique_ptr<ConnectionManager> _connectionManager;
    std::unique_ptr<ReceiverManager>   _receiverManager;
    std::unique_ptr<QSystemTrayIcon>   _systemTrayIcon;
    App::AppState                      _appState;
    const Style::icon*                 _icon;
};
