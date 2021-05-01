#pragma once 
#include <QApplication>
#include <pch.hpp>
#include "ConnectionManager.hpp"
#include "ChatWindow.hpp"
#include "MainWidget.hpp"

namespace AppS
{
    enum class AppStateS
    {
        LoginForm,
        RegistrationForm,
        ChatWindowForm
    };
}

class Application: public QApplication
{

private:
    MainWidget* mMainWidget;
    BioButton* mBioButton;
    AppS::AppStateS mAppState;
    Application* mainObjectApplication;

public:
    Application(int& argc, char** argv);
    void create();
    void show();
    void setAppState(AppS::AppStateS app_state);
    void setObjectApplication(Application* generalWidget);
    Application& getObjectApplication();

};
