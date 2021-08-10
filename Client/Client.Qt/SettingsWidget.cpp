#include "SettingsWidget.hpp"

SettingsWidget::SettingsWidget(QWidget *parent)
    : QWidget(parent), _settings(Settings::getInstance())
{
    this->setWindowTitle("Settings");
    this->setStyleSheet(
        "QWidget {"
        "background-color: #323232;"
        "}");

    _vBoxLayout             = std::make_unique<QVBoxLayout>();
    _chatSettings           = std::make_unique<Label>("Chat Font size" ,this);
    _chatFontSizeSlider     = std::make_unique<QSlider>(Qt::Orientation::Horizontal,this);
    _loadButton             = std::make_unique<FlatButton>(this, "Apply");
    _defaultButton          = std::make_unique<FlatButton>(this, "Reset");

    _chatFontSizeSlider->setRange(_fontSizeMin, _fontSizeMax);

    _vBoxLayout->addWidget(_chatSettings.get());
    _vBoxLayout->addWidget(_chatFontSizeSlider.get());
    _vBoxLayout->addWidget(_loadButton.get());
    _vBoxLayout->addWidget(_defaultButton.get());

    setLayout(_vBoxLayout.get());
    connectUi();
}

void SettingsWidget::connectUi()
{
    _loadButton->setClickCallback([&]() { loadButtonClicked(); });
    _defaultButton->setClickCallback([&]() { defaultButtonClicked(); });
}

void SettingsWidget::loadButtonClicked()
{
    _settings.setFontSize(_chatFontSizeSlider->value());
    _settings.writeSettings();
}

void SettingsWidget::defaultButtonClicked()
{
    _settings.resetSettings();
}
