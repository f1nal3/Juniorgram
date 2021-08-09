#pragma once

#include <QWidget>
#include <memory>

#include "Widgets/LogoWidget.hpp"

class FlatInput;
class FlatButton;
class LinkButton;

/**
 * @class Login
 * @brief Login form for JuniorGram
 */
class Login : public QWidget
{
    Q_OBJECT
public:
    /// Constructor for login form
    explicit Login(QWidget* parent = nullptr);

public slots:
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
