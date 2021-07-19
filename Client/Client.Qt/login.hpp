#pragma once

#include <QWidget>

class FlatInput;
class FlatButton;
class LogoWidget;
class LinkButton;

class Login : public QWidget
{
    Q_OBJECT
public:
    explicit Login(QWidget* parent = nullptr);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    std::unique_ptr<FlatInput>  _usernameInput;
    std::unique_ptr<FlatInput>  _passwordInput;
    std::unique_ptr<FlatButton> _signInButton;
    std::unique_ptr<FlatButton> _registrationButton;
    std::unique_ptr<LinkButton> _forgotPasswordLink;
    std::unique_ptr<LogoWidget> _logoWidget;
};
