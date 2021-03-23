#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent)
{
    QWidget* window = new QWidget;
    setWindowTitle("MainWindow");
    setFixedSize(800,600);

    textEdit = new TextEdit(window);
    sendButton = new QPushButton("Send");
    randomButton = new QPushButton("Bot");
    chatView = new QListView(window);
    model = new QStringListModel;
    messagesList = new QStringList;
    chatView->setModel(model);
    
    QHBoxLayout *layout = new QHBoxLayout;
    QVBoxLayout *textLayout = new QVBoxLayout;
    QVBoxLayout *barLayout = new QVBoxLayout;

    barLayout->addWidget(randomButton);
    barLayout->addWidget(sendButton);
    barLayout->setAlignment(Qt::AlignBottom);

    textLayout->addWidget(chatView, 95);
    textLayout->addWidget(textEdit, 5);

    layout->addLayout(textLayout);
    layout->addLayout(barLayout);

    setLayout(layout);

    connect(sendButton, SIGNAL(clicked()), this, SLOT(updateMessagesList_User()));
    connect(randomButton, SIGNAL(clicked()),this, SLOT(updateMessagesList_Bot()));
    chatView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete model;
    delete messagesList;
    delete chatView;
    delete sendButton;
    delete randomButton;
    delete textEdit;
}

void MainWindow::updateChat() { model->setStringList(*messagesList); }

void MainWindow::updateMessagesList_User()
{
    if (textEdit->text() == "") return;
    *messagesList << textEdit->text();  
    textEdit->clear();
    updateChat();
}

void MainWindow::updateMessagesList_Bot()
{
    messagesList->append("This is message from bot!");
    updateChat();
}
