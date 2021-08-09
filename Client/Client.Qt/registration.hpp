#pragma once
#include <QWidget>
#include <Utility/Utility.hpp>
#include <memory>

class FlatInput;
class FlatButton;
class LogoWidget;

/**
 * @class Registration
 * @brief Registration form for JuniorGram
 */
class Registration : public QWidget
{
    Q_OBJECT
public:
    /// Constructor for registration form
    explicit Registration(QWidget* parent = nullptr);

public slots:
    /// On registration
    void onRegistration(Utility::RegistrationCodes code);

protected:
    /// Handle resize
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
