#pragma once
#include "pch.hpp"

enum class AppState
{
    LoginForm,
    Authorized,
    RegistrationForm,
    ConnectToServer
};

namespace App
{
void create();
void show();
void setAppState(AppState app_state);
}  // namespace App
