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
     * @brief Draft method for setting font size value /
     * if input argument is <=0, size value remains as before
     * @param size as int.
     */
    void setFontSize(int size);
    /**
     * @brief Method for getting font size value if it's present
     * @return fontSize as int /
     * if fontSize is not set returns indicator of optional type /
     * with uninitialized state
     */
    std::optional<int> getFontSize();

private:
    int _fontSize;
    static constexpr int _minFontSize = 0;
};
