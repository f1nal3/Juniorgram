#pragma once
#include "Application.hpp"
#include "Widgets/Buttons.hpp"
#include "Widgets/InputFields.hpp"
#include "Widgets/LogoWidget.hpp"

/**
 * @class Registration
 * @brief Registration form for JuniorGram
 */
class Registration : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for registration form
     * @param parent MainWidget
     */
    explicit Registration(QWidget* parent = nullptr);

protected:
    /**
     * @brief Handle resize events
     * @param event Resize event
     */
    void resizeEvent(QResizeEvent* event) override;

private:
    std::unique_ptr<FlatInput>  _emailInput;
    std::unique_ptr<FlatInput>  _usernameInput;
    std::unique_ptr<FlatInput>  _passwordInput;
    std::unique_ptr<FlatInput>  _repeatPasswordInput;
    std::unique_ptr<FlatButton> _registrationButton;
    std::unique_ptr<FlatButton> _backButton;
    std::unique_ptr<LogoWidget> _logoWidget;
};
