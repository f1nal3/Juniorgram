#pragma once 
#include <QVBoxLayout>
#include <QSlider>
#include <QWidget>
#include <memory>

#include <Settings.hpp>
#include "Style/StyleBasic.hpp"
#include "Widgets/Buttons.hpp"
#include "Widgets/Label.hpp"

/**
 * @class SettingsWidget
 * @brief Widget which provides a way for a user to configure /
 * visual environment of client.
 */
class SettingsWidget : public QWidget
{
    Q_OBJECT
public:
/**
* @brief SettingsWidget constructor
* @param parent as Qwidget
*/
    explicit SettingsWidget(QWidget* parent = nullptr);

private:
    Settings& _settings; 
    std::unique_ptr<Label>       _chatSettings;         
    std::unique_ptr<QSlider>     _chatFontSizeSlider; 
    std::unique_ptr<QVBoxLayout> _vBoxLayout;
    std::unique_ptr<FlatButton>  _loadButton;
    std::unique_ptr<FlatButton>  _defaultButton;
    static constexpr int _fontSizeMin = 6;
    static constexpr int _fontSizeMax = 18;

private:    
    void connectUi();

private slots:
    void loadButtonClicked();
    void defaultButtonClicked();
};
