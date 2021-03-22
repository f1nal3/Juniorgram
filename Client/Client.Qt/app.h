#ifndef JUNIORGRAM_APP_H
#define JUNIORGRAM_APP_H

#include "login.h"
#include "mainwidget.h"
#include "pch.h"
#include "registration.h"

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
