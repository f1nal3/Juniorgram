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
#include <memory>
#include "registration.h"
#include "mainwindow.h"

class Login : public QWidget
{
    Q_OBJECT
public:
    explicit Login(QWidget* parent = nullptr);
public slots:
    void displayRegistrationWindow();
    void displayMainWindow();
private:
    std::unique_ptr<QGridLayout>  gridLayout;
    std::unique_ptr<QLabel>       usernameLabel;
    std::unique_ptr<QLabel>       passwordLabel;
    std::unique_ptr<QLineEdit>    usernameLineEdit;
    std::unique_ptr<QLineEdit>    passwordLineEdit;
    std::unique_ptr<QPushButton>  buttonSignin;
    std::unique_ptr<QPushButton>  buttonRegistration;
    std::unique_ptr<Registration> registrationWindow;
    std::unique_ptr<MainWindow>   mainWindow;
};
