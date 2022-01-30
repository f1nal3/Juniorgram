#include "Settings.hpp"
#include "ServerInfo.hpp"

Settings::Settings(const QString& filename, QSettings::Format format)
    : QSettings(filename, format)
{
}

Settings& Settings::getInstance()
{
    static Settings instance{ defaultFilename.data(), QSettings::IniFormat };
    return instance;
}

void Settings::writeSettings()
{
    beginGroup("Font");
    setValue("ChatFontSize", _fontSize);
    endGroup();
}

void Settings::resetSettings()
{
    clear();
}

void Settings::setFontSize(std::int32_t size)
{
    if (size > _minFontSize) 
        _fontSize = size;    
}

std::optional<std::int32_t> Settings::getFontSize()
{
    if (contains("Font/ChatFontSize")) 
    { 
        return value("Font/ChatFontSize").toInt(); 
    }
    return { };
}

void Settings::configureSettings(const QString& groupName, const std::map<QString, QVariant>& values)
{
    std::unique_lock<std::mutex> lock(_mutex);
    beginGroup(groupName);
    for (const auto& [key, value] : values)
    {
        if (!contains(key))
        {
            setValue(key, value);
        }
    }
    endGroup();
}

void Settings::rewriteSetting(const QString& fullName, const QVariant& newValue)
{
    std::unique_lock<std::mutex> lock(_mutex);
    if (!contains(fullName))
    {
        throw Errors{ "Key " + fullName.toStdString() + " doesn't exist ", fullName, newValue };
    }
    setValue(fullName, value);
}
