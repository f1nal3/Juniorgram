#include "ChatWindow.hpp"
#include "app.h"

ChatWindow::ChatWindow(QWidget* parent) : QWidget(parent)
{
    mainLayout          = new QVBoxLayout(this);
    messageEditLayout   = new QHBoxLayout();
    messageButtonLayout = new QVBoxLayout();
    chatWidget          = new QListWidget(this);
    sendButton          = new QPushButton("Send");
    botButton           = new QPushButton("Bot");
    textEdit            = new TextEdit(this);
    verticalUpSpacer    = new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalDownSpacer  = new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    mainLayout->addWidget(chatWidget, 70);

    messageButtonLayout->addItem(verticalUpSpacer);
    messageButtonLayout->addWidget(sendButton);
    messageButtonLayout->addWidget(botButton);
    messageButtonLayout->addItem(verticalUpSpacer);

    messageEditLayout->addWidget(textEdit);
    messageEditLayout->addLayout(messageButtonLayout);

    mainLayout->addLayout(messageEditLayout, 30);

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
    if (textEdit->text() != "") newMessage(textEdit->text());
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
