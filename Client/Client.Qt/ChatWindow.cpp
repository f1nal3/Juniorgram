#include "ChatWindow.h"
#include "app.h"

ChatWindow::ChatWindow(QWidget* parent) : QWidget(parent)
{
    QWidget* window = new QWidget;

    textEdit = new TextEdit(window);
    sendButton = new QPushButton("Send");
    randomButton = new QPushButton("Bot");
    chatWidget = new QListWidget(window);

    QHBoxLayout *layout = new QHBoxLayout;
    QVBoxLayout *textLayout = new QVBoxLayout;
    QVBoxLayout *barLayout = new QVBoxLayout;

    barLayout->addWidget(randomButton);
    barLayout->addWidget(sendButton);
    barLayout->setAlignment(Qt::AlignBottom);
    

    textLayout->addWidget(chatWidget, 95);
    textLayout->addWidget(textEdit, 5);

    layout->addLayout(textLayout);
    layout->addLayout(barLayout);

    setLayout(layout);
}

ChatWindow::~ChatWindow(){
    delete sendButton;
    delete randomButton;
    delete textEdit;
    delete chatWidget;
}
