#include "mainwindow.h"

#include <Serialize.hpp>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    messagesList = new QStringList();
    model        = new QStringListModel(*messagesList);

    ui->listView_chatView->setModel(model);
    connect(ui->pushButton_Submit, &QPushButton::released, this,
            &MainWindow::updateMessagesList_User);
    connect(ui->pushButton_BotMessage, &QPushButton::released, this,
            &MainWindow::updateMessagesList_Bot);
}

MainWindow::~MainWindow()
{
    delete model;
    delete messagesList;
    delete ui;
}

void MainWindow::updateChat() { model->setStringList(*messagesList); }

void MainWindow::updateMessagesList_User()
{
    if (ui->lineEdit->text() == "") return;
    *messagesList << ui->lineEdit->text();
    ui->lineEdit->clear();
    updateChat();
}

void MainWindow::updateMessagesList_Bot()
{
    Serialize gg;
    messagesList->append("This is message from bot!" + QString::number(gg.getMagicNumber()));
    updateChat();
}
