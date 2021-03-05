#include "registration.h"
#include "ui_registration.h"

registration::registration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registration)
{
    ui->setupUi(this);
}

registration::~registration()
{
    delete ui;
}

void registration::on_pushButton_createAccount_clicked()
{
    QString registrationUsername = ui->lineEdit_username->text();
    QString registrationPassword = ui->lineEdit_password->text();
    QString registrationRepeatPassword = ui->lineEdit_repeatPassword->text();
}
