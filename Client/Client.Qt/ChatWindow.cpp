#include "ChatWindow.hpp"
#include "app.h"

ChatWindow::ChatWindow(QWidget* parent) : QWidget(parent)
{
    // auto* window = new QWidget;
    textEdit     = new TextEdit(this);
    sendButton   = new FlatButton("Send");
    randomButton = new FlatButton("Bot");
    chatWidget   = new QListWidget(this);

    auto* layout     = new QVBoxLayout;
    auto* textLayout = new QHBoxLayout;
    auto* barLayout  = new QVBoxLayout;

    barLayout->addWidget(randomButton);
    barLayout->addWidget(sendButton);
    barLayout->setAlignment(Qt::AlignRight);


    textLayout->addWidget(textEdit);
    textLayout->addLayout(barLayout);

    layout->addWidget(chatWidget);
    layout->addLayout(textLayout);

    connectButton();

    setLayout(layout);
}


void ChatWindow::connectButton()
{
    connect(sendButton, &QPushButton::released, this, &ChatWindow::updateMessagesList_User);
    connect(randomButton, &QPushButton::released, this, &ChatWindow::updateMessagesList_Bot);
}


void ChatWindow::updateMessagesList_User()
{
    newMessage(textEdit->text());
    textEdit->clear();
}

void ChatWindow::updateMessagesList_Bot()
{
    newMessage("This is message from bot!", "bot");
}

void ChatWindow::newMessage(QString textMessage, QString userNameMessage)
{
    MessageWidget* myItem = new MessageWidget(textMessage, userNameMessage);
    QListWidgetItem* item  = new QListWidgetItem();
    item->setSizeHint(QSize(0, 120));
    chatWidget->addItem(item);
    chatWidget->setItemWidget(item, myItem);
}

void ChatWindow::newMessage(QString textMessage)
{
    MessageWidget* myItem = new MessageWidget(textMessage);
    QListWidgetItem* item  = new QListWidgetItem();
    item->setSizeHint(QSize(0, 120));
    chatWidget->addItem(item);
    chatWidget->setItemWidget(item, myItem);
}

ChatWindow::~ChatWindow(){
    delete sendButton;
    delete randomButton;
    delete textEdit;
    delete chatWidget;
}
