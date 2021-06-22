#pragma once
#include "Widgets/FlatButton.hpp"
#include "Widgets/InputFields.hpp"
#include "Widgets/LogoWidget.hpp"
#include "pch.hpp"
#include "Application.hpp"

class Registration : public QWidget
{
    Q_OBJECT
public:
    explicit Registration(QWidget* parent = nullptr);
    ~Registration();
protected:
    void keyPressEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    FlatInput* usernameLineEdit;
    FlatInput* passwordLineEdit;
    FlatInput* passwordRepeatLineEdit;
    FlatButton* registrationButton;
    FlatButton* back;
    LogoWidget* logoWidget;
};
