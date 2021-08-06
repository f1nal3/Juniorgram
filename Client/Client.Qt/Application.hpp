#pragma once
#include <QApplication>
#include <memory>

#include "ConnectionManager.hpp"
#include "Style/Style.hpp"

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
    /**
     * @brief Creates window and initialize all stuff
     */
    void create();
    /**
     * @brief Shows the window
     */
    void show();
    /**
     * @brief Change app state
     * @param app_state App state
     */
    void setAppState(App::AppState app_state);

    std::unique_ptr<ConnectionManager>& connectionManager();

private:
    std::unique_ptr<MainWidget>        _mainWidget;
    std::unique_ptr<ConnectionManager> _connectionManager;
    App::AppState                      _appState;
    const Style::icon*                 _icon;
};
