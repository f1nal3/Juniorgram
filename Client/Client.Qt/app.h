#pragma once
#include "pch.h"

enum class AppState
{
    LoginForm,
    Authorized,
    RegistrationForm
};

namespace App
{
void create();
void show();
void setAppState(AppState app_state);
}  // namespace App
