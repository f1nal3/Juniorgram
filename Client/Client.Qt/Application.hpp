#pragma once 
#include <QApplication>
#include <pch.hpp>
#include "ConnectionManager.hpp"
#include "ChatWindow.hpp"
#include "MainWidget.hpp"
#include "login.hpp"
#include "registration.hpp"

class Application: public QApplication
{

private:
    enum class AppState
    {
        LoginForm,
        AuthorizedForm,
        RegistrationForm,
        ChatWindowForm
    };
    MainWidget* mMainWidget;
    BioButton* mBioButton;
    AppState mAppState = AppState::RegistrationForm;
    Application *mainObjectWidget;

public:
    Application(int& argc, char** argv);
    void create();
    void show();
    void setAppState(AppState app_state);
    void setObjectApplication(Application* generalWidget);
    Application& getObjectApplication();

};
