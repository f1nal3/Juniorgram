#include "ChatWindow.hpp"
#include "app.h"

ChatWindow::ChatWindow(QWidget* parent) : QWidget(parent)
{
    QWidget* window = new QWidget;

    textEdit     = new TextEdit(window);
    sendButton   = new QPushButton("Send");
    randomButton = new QPushButton("Bot");
    chatWidget   = new QListWidget(window);

    QHBoxLayout* layout     = new QHBoxLayout;
    QVBoxLayout* textLayout = new QVBoxLayout;
    QVBoxLayout* barLayout  = new QVBoxLayout;

    barLayout->addWidget(randomButton);
    barLayout->addWidget(sendButton);
    barLayout->setAlignment(Qt::AlignBottom);

    textLayout->addWidget(chatWidget, 95);
    textLayout->addWidget(textEdit, 5);

    layout->addLayout(textLayout);
    layout->addLayout(barLayout);

    connectButton();
    setLayout(mainLayout);
}

void ChatWindow::keyPressEvent(QKeyEvent* event)
{
    if ((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) &&
        (textEdit->text() != ""))
    {
        newMessage(textEdit->text());
        textEdit->clear();
    }
}

void ChatWindow::connectButton()
{
    connect(sendButton, &QPushButton::released, this, &ChatWindow::updateMessagesList_User);
    connect(botButton, &QPushButton::released, this, &ChatWindow::updateMessagesList_Bot);
}


void ChatWindow::updateMessagesList_User()
{
    if (textEdit->text() != "")
        newMessage(textEdit->text());
    textEdit->clear();
}

void ChatWindow::updateMessagesList_Bot() { newMessage("This is message from bot!", "bot"); }

void ChatWindow::newMessage(QString textMessage, QString userNameMessage)
{
    MessageWidget* myItem = new MessageWidget(textMessage, userNameMessage);
    QListWidgetItem* item = new QListWidgetItem();
    item->setSizeHint(QSize(0, 120));
    chatWidget->addItem(item);
    chatWidget->setItemWidget(item, myItem);
}

void ChatWindow::newMessage(QString textMessage)
{
    MessageWidget* myItem = new MessageWidget(textMessage);
    QListWidgetItem* item = new QListWidgetItem();
    item->setSizeHint(QSize(0, 120));
    chatWidget->addItem(item);
    chatWidget->setItemWidget(item, myItem);
}

ChatWindow::~ChatWindow()
{
    delete mainLayout;
    delete messageEditLayout;
    delete messageButtonLayout;
    delete sendButton;
    delete botButton;
    delete textEdit;
    delete chatWidget;
    delete verticalUpSpacer;
    delete verticalDownSpacer;
}
