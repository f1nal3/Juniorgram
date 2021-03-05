#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "mainwindow.h"
#include "registration.h"
#include <QMessageBox>

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private slots:
    void on_pushButton_login_clicked();

    void on_pushButton_registration_clicked();

private:
    Ui::login *ui;
    MainWindow *m_mainWindow;
    registration *m_registration;
};

#endif // LOGIN_H
