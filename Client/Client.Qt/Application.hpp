#pragma once
#include <QApplication>
#include <memory>

#include "MainWidget.hpp"

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

class BioButton;

class Application : public QApplication
{
private:
    std::unique_ptr<MainWidget> _mainwidget;
    App::AppState               _appState;
    const Style::icon*          _icon;

public:
    Application(int& argc, char** argv);
    void create();
    void show();
    void setAppState(App::AppState app_state);
};
