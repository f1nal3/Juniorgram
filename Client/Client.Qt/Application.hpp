#pragma once
#include <DataAccess.SQLite/LiteRepositoryContainer.hpp>
#include <QApplication>
#include <Style/Styles.hpp>
#include <memory>

#include "ConnectionManager.hpp"

class MainWidget;

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
    /**
     * @brief Change app state
     * @param appState App state
     */
    void setAppState(App::AppState appState);

    /// Connection manager
    std::unique_ptr<ConnectionManager>& connectionManager();
    /// Repository container
    std::unique_ptr<DataAccess::LiteRepositoryContainer>& repositoryContainer();
public slots:
    /// Reconnects to server
    void reconnectToServer();

private:
    std::unique_ptr<DataAccess::LiteRepositoryContainer> _repContainer;

    std::unique_ptr<MainWidget>        _mainWidget;
    std::unique_ptr<ConnectionManager> _connectionManager;
    std::unique_ptr<ReceiverManager>   _receiverManager;
    App::AppState                      _appState;
    const Style::icon*                 _icon;
};
