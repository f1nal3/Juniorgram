#pragma once
#include "Widgets/FlatButton.hpp"
#include "Widgets/InputFields.hpp"
#include "Widgets/LogoWidget.hpp"
#include "pch.hpp"
#include "Application.hpp"

class Login : public QWidget
{
    Q_OBJECT
public:
    explicit Login(QWidget* parent = nullptr);
    ~Login();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    FlatInput* usernameLineEdit;
    FlatInput* passwordLineEdit;
    FlatButton* buttonSignin;
    FlatButton* buttonRegistration;
    LogoWidget* logoWidget;
};
