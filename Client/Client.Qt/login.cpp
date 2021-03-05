#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

void login::on_pushButton_login_clicked()
{
   QString loginUsername = ui->lineEdit_username->text();
   QString loginPassword = ui->lineEdit_password->text();
   if(loginUsername == "test" && loginPassword == "test")
   {
       hide();
       m_mainWindow = new MainWindow(this);
       m_mainWindow->show();
   }
   else
   {
       QMessageBox::information(this, "Login", "Username or password not is correct");
   }
}

void login::on_pushButton_registration_clicked()
{
   hide();
   m_registration = new registration(this);
   m_registration->show();
}
