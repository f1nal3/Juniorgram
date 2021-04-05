#include "mainwindow.h"
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
    ui->listView_chatView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::newMessage(QString textMessage, QString userNameMessage)
{
    QMessageWidget* myItem = new QMessageWidget(textMessage, userNameMessage);
    QListWidgetItem* item  = new QListWidgetItem();
    item->setSizeHint(QSize(0, 120));
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, myItem);
}

void MainWindow::newMessage(QString textMessage)
{
    QMessageWidget* myItem = new QMessageWidget(textMessage);
    QListWidgetItem* item  = new QListWidgetItem();
    item->setSizeHint(QSize(0, 120));
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, myItem);
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
    newMessage(ui->lineEdit->text());
    ui->lineEdit->clear();
    updateChat();
}

void MainWindow::updateMessagesList_Bot()
{
    messagesList->append("This is message from bot!");
    newMessage("This is message from bot!", "bot");
    updateChat();
}
