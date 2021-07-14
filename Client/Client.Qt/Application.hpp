#pragma once
#include <QApplication>

#define oApp (static_cast<Application*>(QCoreApplication::instance()))

namespace App
{
enum class AppState
{
    LoginForm,
    RegistrationForm,
    ChatWindowForm
};
}

class MainWidget;
class BioButton;

class Application : public QApplication
{
private:
    MainWidget*   _mainwidget;
    BioButton*    _bio;
    App::AppState mAppState;

public:
    Application(int& argc, char** argv);
    void create();
    void show();
    void setAppState(App::AppState app_state);
};
