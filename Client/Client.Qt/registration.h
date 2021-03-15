#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QFormLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QAction>
#include <QGridLayout>
#include <QProcess>
#include <memory>

class Login;

class Registration : public QWidget
{
    Q_OBJECT
public:
    explicit Registration(QWidget* parent = nullptr);
public slots:
   void displayLoginWindow();
private:
    std::unique_ptr<QLabel>      usernameLabel;
    std::unique_ptr<QLabel>      passwordLabel;
    std::unique_ptr<QLabel>      passwordRepeatLabel;
    std::unique_ptr<QLineEdit>   usernameLineEdit;
    std::unique_ptr<QLineEdit>   passwordLineEdit;
    std::unique_ptr<QLineEdit>   passwordRepeatLineEdit;
    std::unique_ptr<QPushButton> registrationButton;
    std::unique_ptr<QGridLayout> gridLayout;
    std::unique_ptr<Login>       loginWindow;
};
