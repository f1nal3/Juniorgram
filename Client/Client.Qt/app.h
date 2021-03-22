#ifndef JUNIORGRAM_APP_H
#define JUNIORGRAM_APP_H

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

#endif  // JUNIORGRAM_APP_H
