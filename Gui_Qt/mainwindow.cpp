#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serialize = new Serialize();
    messagesList = new QStringList();
    serialize->updateSerialize(*messagesList);
    model        = new QStringListModel(*messagesList);
    ui->listView_chatView->setModel(model);
    connect(ui->pushButton_Submit, &QPushButton::released, this,
            &MainWindow::updateMessagesList_User);
    connect(ui->pushButton_BotMessage, &QPushButton::released, this,
            &MainWindow::updateMessagesList_Bot);
    ui->listView_chatView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete model;
    delete messagesList;
    delete ui;
    delete serialize;
}

void MainWindow::updateChat() { model->setStringList(*messagesList); }

void MainWindow::updateMessagesList_User()
{
    if (ui->lineEdit->text() == "") return;
    *messagesList << ui->lineEdit->text();  
    serialize->pushData(*messagesList);
    ui->lineEdit->clear();
    updateChat();
}

void MainWindow::updateMessagesList_Bot()
{
    messagesList->append("This is message from bot!");
    serialize->pushData(*messagesList);
    updateChat();
}
