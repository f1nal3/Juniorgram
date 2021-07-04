#include "ChatWidget.hpp"

#include "ChatHistory.hpp"

ChatWidget::ChatWidget(QWidget* parent) : QWidget(parent)
{
    setContentsMargins(0, 0, 0, 0);
    mainChatLayout = new QVBoxLayout(this);
    mainChatLayout->setContentsMargins(0, 0, 0, 0);
    mainChatLayout->setMargin(0);
    mainChatLayout->setSpacing(0);
    chatWidget = new ChatHistory(this);
    textEdit   = new TextEdit(chatWidget);
    setMinimumWidth(Style::valueDPIScale(400));

    mainChatLayout->addWidget(chatWidget, 85);
    mainChatLayout->addWidget(textEdit, 15);
    setLayout(mainChatLayout);
    connectUi();
}

void ChatWidget::newMessage(const QString& messageText)
{
    for (int i = 0; i < 512; i++)
    {
        chatWidget->addMessage(messageText + QString("%1").arg(i),
                               QDateTime::currentSecsSinceEpoch());
    }
}

void ChatWidget::newMessage(QString messageText, QString userNameMessage)
{
    chatWidget->addMessage(messageText, QDateTime::currentSecsSinceEpoch(), userNameMessage);
}

void ChatWidget::connectUi()
{
    connect(textEdit, SIGNAL(sendMessageSignal(QString)), this, SLOT(newMessage(QString)));
}

ChatWidget::~ChatWidget()
{
    delete mainChatLayout;
    delete chatWidget;
    delete textEdit;
}
