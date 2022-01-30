#pragma once

#include <QSettings>
#include <optional>
#include <string_view>
#include <mutex>
#include <stdexcept>

/** @class Settings
 *  @brief Persistent settings
 */
class Settings : public QSettings
{
private:
    Settings(const QString& filename, QSettings::Format format);

    Settings(const Settings&) = delete;
    Settings(Settings&&) = delete;
    Settings& operator=(const Settings&) = delete;
    Settings& operator=(Settings&&) = delete;

public:
    /**
     * @brief Method for getting reference to Settings instance.
     * @return Reference to Settings instance.
     */
    static Settings& getInstance();
    /**
     * @brief Draft method for writting settings keys and /
     * values to .ini file.
     */
    void writeSettings();
    /**
     * @brief Method for Settiings reset by clearing .ini file
     */
    void resetSettings();
    /**
     * @brief Draft method for setting font size value /
     * if it is more than const _minFontSize = 0
     * @param size as std::int32_t.
     */
    void setFontSize(std::int32_t size);
    /**
     * @brief Method for getting font size value if it's present
     * @return fontSize as std::int32_t /
     * if fontSize is not set returns indicator of optional type /
     * with uninitialized state
     */
    std::optional<std::int32_t> getFontSize();

    void configureSettingsGroup(const QString& groupName, const std::map<QString, QVariant>& values);
    void configureSettingsGroup(const std::map<QString, QVariant>& values) { configureSettingsGroup("General", values); }

    void rewriteSetting(const QString& fullName, const QVariant& newValue);

    static constexpr std::string_view defaultFilename = "juniorgram_settings.ini";

    class Errors;

private:

    std::mutex _mutex;

    std::int32_t _fontSize;
    static constexpr std::int32_t _minFontSize = 0;
};

class Settings::Errors : public std::runtime_error
{
    QString  _fullKey;
    QVariant _value; 
public:
    Errors(std::string wht): std::runtime_error(std::move(wht)) { }
    Errors(std::string wht, QString fullKey, QVariant value): std::runtime_error(std::move(wht)), _fullKey(fullKey), _value(value) { }

    const QString&  getKey()   const { return _fullKey; }
    const QVariant& getValue() const { return _value; }
};
