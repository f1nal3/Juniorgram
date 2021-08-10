#include "Settings.hpp"

Settings::Settings()
{
    settings = std::make_unique<QSettings>();
}

std::unique_ptr<Settings> Settings::instance = nullptr;

Settings& Settings::getInstance()
{
    if(instance == nullptr)
    {
        instance.reset(new Settings());
    }
    return *instance;
}

void Settings::writeSettings()
{
    settings->beginGroup("Font");
    settings->setValue("ChatFontSize", _fontSize);
    settings->endGroup();
}

void Settings::resetSettings()
{
    settings->clear();
}

void Settings::setFontSize(int size)
{
    _fontSize = size;
}

int Settings::getFontSize()
{
    if(!settings->contains("Font/ChatFontSize")) { return -1; }
    return settings->value("Font/ChatFontSize").toInt();
}

