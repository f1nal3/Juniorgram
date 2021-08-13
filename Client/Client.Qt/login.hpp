#pragma once

#include <QWidget>
#include <Widgets/Page.hpp>
#include <memory>

#include "Widgets/LogoWidget.hpp"

class FlatInput;
class FlatButton;
class LinkButton;

/**
 * @class Login
 * @brief Login page for JuniorGram
 */
class Login : public Page
{
    Q_OBJECT
public:
    /// Constructor for login page
    explicit Login(QWidget* parent = nullptr);

private slots:
    /// On login handler
    void onLogin();

protected:
    /// Handle resize
    void resizeEvent(QResizeEvent* event) override;

private:
    std::unique_ptr<FlatInput>  _usernameInput;
    std::unique_ptr<FlatInput>  _passwordInput;
    std::unique_ptr<FlatButton> _signInButton;
    std::unique_ptr<FlatButton> _registrationButton;
    std::unique_ptr<LinkButton> _forgotPasswordLink;
    std::unique_ptr<LogoWidget> _logoWidget;
};
