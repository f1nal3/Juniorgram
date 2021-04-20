#include "ChatWindow.hpp"

#include <utility>

#include "App.hpp"
#include "Style/Style.hpp"

ChatWindow::ChatWindow(QWidget* parent) : QWidget(parent)
{
    hBoxLayout          = new QHBoxLayout(this);
    mainLayout          = new QVBoxLayout();
    messageEditLayout   = new QHBoxLayout();
    messageButtonLayout = new QVBoxLayout();
    channelListWidget   = new ChannelListWidget();
    chatWidget          = new QListWidget(this);
    sendButton          = new FlatButton("Send");
    botButton           = new FlatButton("Bot");
    textEdit            = new TextEdit(this);
    verticalUpSpacer    = new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalDownSpacer  = new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    mainLayout->addWidget(chatWidget, 85);

    messageButtonLayout->addItem(verticalUpSpacer);
    messageButtonLayout->addWidget(sendButton);
    messageButtonLayout->addWidget(botButton);
    messageButtonLayout->addItem(verticalDownSpacer);

    messageEditLayout->addWidget(textEdit);
    messageEditLayout->addLayout(messageButtonLayout);

    mainLayout->addLayout(messageEditLayout, 15);

    hBoxLayout->addWidget(channelListWidget);
    hBoxLayout->addLayout(mainLayout);

    connectButton();
    setLayout(hBoxLayout);
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
    delete mainLayout;
    delete sendButton;
    delete botButton;
    delete textEdit;
    delete chatWidget;
    delete hBoxLayout;
    delete channelListWidget;
}
