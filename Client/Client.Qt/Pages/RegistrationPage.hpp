#pragma once
#include <QWidget>
#include <QShortcut>
#include <QLabel>
#include <Utility/Utility.hpp>
#include <Widgets/Page.hpp>
#include <memory>
#include <Widgets/PopupSystemMessage.hpp>

class FlatInput;
class FlatButton;
class LogoWidget;

/**
 * @class RegistrationPage
 * @brief Registration page for JuniorGram
 */
class RegistrationPage : public Page
{
    Q_OBJECT
public:
    /// Constructor for registration page
    explicit RegistrationPage(QWidget* parent = nullptr);

private slots:
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
    std::unique_ptr<PopupSystemMessage> _popupSystemMessage;

    std::unique_ptr<QShortcut>  _registrationHotkey;
};