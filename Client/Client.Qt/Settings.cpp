#include "Settings.hpp"
#include <QDebug>
#include <iostream>

Settings::Settings()
{
    settings = std::make_unique<QSettings>();
}

std::unique_ptr<Settings> Settings::instance = nullptr;

Settings &Settings::getInstance()
{
    if(instance == nullptr)
    {
        QCoreApplication::setOrganizationName("L&D C++ Lab");
        QCoreApplication::setApplicationName("Juniorgram");
        instance.reset(new Settings());
        qDebug() << instance->fileName(); 
    }
    return *instance;
}

Settings::~Settings()
{
    std::cout<<"Settings Destructor\n";
    if(instance.release() == nullptr){
        std::cout<<"instance killed\n";
    }
    else{
        std::cout<<"memory leak\n";
    }
}

void Settings::WriteSettings()
{
    settings->beginGroup("Font");
    settings->setValue("FontSize", fontSize);
    settings->endGroup();
}

void Settings::setFontSize(int size)
{
    fontSize = size;
}

int Settings::getFontSize()
{
    return fontSize;
}

