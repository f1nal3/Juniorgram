#pragma once
#include <QApplication>
#include <QSettings>
#include <memory>

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
    static Settings &getInstance();
    ~Settings();
    void WriteSettings();
    void setFontSize(int size);
    int getFontSize();

private:
    int fontSize;
};