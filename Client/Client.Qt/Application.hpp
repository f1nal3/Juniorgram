#pragma once 
#include <QApplication>
#include <pch.hpp>
#include "ConnectionManager.hpp"
#include "ChatWindow.hpp"
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


class Application : public QApplication
{
private:
    MainWidget* mMainWidget;
    BioButton* mBioButton;
    App::AppState mAppState;
    Application* mainObjectApplication;

public:
    Application(int& argc, char** argv);
    void create();
    void show();
    void setAppState(App::AppState app_state);
};
