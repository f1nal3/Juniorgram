#pragma once
#include "Application.hpp"
#include "Widgets/FlatButton.hpp"
#include "Widgets/InputFields.hpp"
#include "Widgets/LogoWidget.hpp"
#include "Widgets/ScrollArea.hpp"

class Login : public QWidget
{
    Q_OBJECT
public:
    explicit Login(QWidget* parent = nullptr);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    std::unique_ptr<FlatInput>  usernameLineEdit;
    std::unique_ptr<FlatInput>  passwordLineEdit;
    std::unique_ptr<FlatButton> buttonSignin;
    std::unique_ptr<FlatButton> buttonRegistration;
    std::unique_ptr<LinkButton> forgotPassword;
    std::unique_ptr<LogoWidget> logoWidget;
};
