#include "ChatWindow.hpp"

#include <utility>

#include "App.hpp"
#include "Style/Style.hpp"

ChatWindow::ChatWindow(QWidget* parent) : QWidget(parent)
{
    mainLayout = new QHBoxLayout(this);
    rightLayout = new QVBoxLayout();
    messageLayout = new QVBoxLayout();
    messageButtonLayout = new QHBoxLayout();
    channelListWidget = new ChannelListWidget();
    chatWidget        = new QListWidget();
    messageTextEdit   = new FlatPlainTextEdit;
    textEdit          = new TextEdit(messageTextEdit);
    sendButton        = new FlatButton("Send");
    botButton         = new FlatButton("Bot");
    horizontalButtonSpacer = new QSpacerItem(40, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    
    mainLayout->addWidget(channelListWidget, 10);
    mainLayout->addLayout(rightLayout, 90);
    rightLayout->addWidget(chatWidget, 85);
    rightLayout->addLayout(messageLayout, 15);
    messageLayout->addWidget(messageTextEdit);
    messageLayout->addLayout(messageButtonLayout);
    messageButtonLayout->addWidget(textEdit);
    messageButtonLayout->addItem(horizontalButtonSpacer);
    messageButtonLayout->addWidget(sendButton);
    messageButtonLayout->addWidget(botButton);
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
    connect(chatWidget, SIGNAL(itemClicked(QListWidgetItem*)), this,
            SLOT(deletingSelection(QListWidgetItem*)));
}

void ChatWindow::deletingSelection(QListWidgetItem* item) 
{ 
    item->setSelected(false); 
}

void ChatWindow::updateMessagesList_User()
{
    if (textEdit->text() != "") newMessage(textEdit->text());
    textEdit->clear();
}

void ChatWindow::updateMessagesList_Bot() { newMessage("This is message from bot!", "bot"); }

void ChatWindow::newMessage(QString textMessage, QString userNameMessage)
{
    auto* item   = new QListWidgetItem();
    auto* myItem = new MessageWidget(std::move(textMessage), std::move(userNameMessage), item);
    myItem->setThisItem(item);
    item->setSizeHint(QSize(0, Style::valueDPIScale(150)));
    chatWidget->addItem(item);
    chatWidget->setItemWidget(item, myItem);
}

void ChatWindow::newMessage(QString textMessage)
{
    auto* item   = new QListWidgetItem();
    auto* myItem = new MessageWidget(std::move(textMessage), item);
    myItem->setThisItem(item);
    item->setSizeHint(QSize(0, Style::valueDPIScale(150)));
    chatWidget->addItem(item);
    chatWidget->setItemWidget(item, myItem);
}

ChatWindow::~ChatWindow()
{
    delete messageButtonLayout;
    delete channelListWidget;
    delete sendButton;
    delete botButton;
    delete messageTextEdit;
    delete textEdit;
    delete chatWidget;
    delete horizontalButtonSpacer;
    delete messageLayout;
    delete rightLayout;
    delete mainLayout;
}
