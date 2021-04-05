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

    setLayout(layout);
}

ChatWindow::~ChatWindow()
{
    delete sendButton;
    delete randomButton;
    delete textEdit;
    delete chatWidget;
}
