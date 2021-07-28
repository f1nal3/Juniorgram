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
        QCoreApplication::setOrganizationName("L&D C++ Lab");
        QCoreApplication::setApplicationName("Juniorgram");
        instance.reset(new Settings());
    }
    return *instance;
}

void Settings::writeSettings()
{
    settings->beginGroup("Font");
    settings->setValue("FontSize", _fontSize);
    settings->endGroup();
}

void Settings::setFontSize(int size)
{
    _fontSize = size;
}

int Settings::getFontSize()
{
    return _fontSize;
}

