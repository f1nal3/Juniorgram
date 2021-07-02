#pragma once
#include "Application.hpp"
#include "Widgets/FlatButton.hpp"
#include "Widgets/InputFields.hpp"
#include "Widgets/LogoWidget.hpp"

class Registration : public QWidget
{
    Q_OBJECT
public:
    explicit Registration(QWidget* parent = nullptr);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    std::unique_ptr<FlatInput>  emailLineEdit;
    std::unique_ptr<FlatInput>  usernameLineEdit;
    std::unique_ptr<FlatInput>  passwordLineEdit;
    std::unique_ptr<FlatInput>  passwordRepeatLineEdit;
    std::unique_ptr<FlatButton> registrationButton;
    std::unique_ptr<FlatButton> back;
    std::unique_ptr<LogoWidget> logoWidget;
};
