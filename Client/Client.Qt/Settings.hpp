#pragma once
#include <QApplication>
#include <QSettings>
#include <memory>

/** @class Settings
 *  @brief Persistent settings
 */
class Settings : public QSettings{
private:
    Settings();
    Settings(const Settings&) = delete;
    Settings(Settings&&) = delete;
    Settings& operator=(const Settings&) = delete;
    Settings& operator=(Settings&&) = delete;
    static std::unique_ptr<Settings> instance;
    std::unique_ptr<QSettings> settings;

public:
    /**
     * @brief Method for getting reference to Settings instance.
     * @return Reference to Settings instance.
     */
    static Settings& getInstance();
    /**
     * @brief Destructor.
     */
    ~Settings(){};
    /**
     * @brief Draft method for writting settings keys and /
     * values to .config file.
     */
    void writeSettings();
    /**
     * @brief Method for Settiings reset by clearing .config file
     */
    void resetSettings();
    /**
     * @brief Draft method for setting font size value
     * @param size as int.
     */
    void setFontSize(int size);
    /**
     * @brief Draft method for getting font size value
     * @return fontSize as int /
     * if fontSize is not set returns -1
     */
    int getFontSize();

private:
    int _fontSize;
};
