#include "ChatWidget.hpp"

ChatWidget::ChatWidget(QWidget* parent) : QWidget(parent)
{
    setContentsMargins(0, 0, 0, 0);
    mainChatLayout = new QVBoxLayout(this);
    mainChatLayout->setContentsMargins(0, 0, 0, 0);
    mainChatLayout->setMargin(0);
    mainChatLayout->setSpacing(0);
    chatWidget     = new QListWidget();
    chatWidget->setLayoutMode(QListView::Batched);
    chatWidget->setMovement(QListView::Free);
    chatWidget->setStyleSheet(
        QString("QListWidget{ "
                "border: 0px;"
                "background: #323232;"
                "}"));
    chatWidget->setDragEnabled(false);
    textEdit       = new TextEdit(&messageTextEdit, &sendButton);

    mainChatLayout->addWidget(chatWidget);  //, 85);
    mainChatLayout->addWidget(textEdit);    //, 15);
    setLayout(mainChatLayout);
    connectUi();
}

ChatWidget::~ChatWidget() 
{ 
    delete mainChatLayout;
    delete chatWidget;
    delete textEdit;
}

void ChatWidget::keyPressEvent(QKeyEvent* event)
{
    if ((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) &&
        (textEdit->getText() != ""))
    {
        newMessage(textEdit->getText());
        textEdit->clear();
    }
}

void ChatWidget::connectUi()
{
    sendButton->setClickCallback([&]() { updateMessagesList_User(); });
    connect(chatWidget, SIGNAL(itemClicked(QListWidgetItem*)), this,
            SLOT(deletingSelection(QListWidgetItem*)));
    connect(chatWidget->model(), SIGNAL(rowsInserted(QModelIndex, int, int)), chatWidget,
            SLOT(scrollToBottom()));
}

void ChatWidget::deletingSelection(QListWidgetItem* item) { item->setSelected(false); }

void ChatWidget::updateMessagesList_User()
{
    if (textEdit->getText() != "") newMessage(textEdit->getText());
    textEdit->clear();
}

void ChatWidget::newMessage(QString textMessage, QString userNameMessage)
{
    auto* item = new QListWidgetItem();
    item->setSizeHint(QSize(0, Style::valueDPIScale(150)));
    auto* myItem =
        new MessageWidget(std::move(textMessage), std::move(userNameMessage), item, false);
    myItem->setThisItem(item);
    chatWidget->addItem(item);
    chatWidget->setItemWidget(item, myItem);
}

void ChatWidget::newMessage(QString textMessage)
{
    auto* item = new QListWidgetItem();
    item->setSizeHint(QSize(0, Style::valueDPIScale(150)));
    auto* myItem = new MessageWidget(std::move(textMessage), item, false);
    myItem->setThisItem(item);
    chatWidget->addItem(item);
    chatWidget->setItemWidget(item, myItem);
}
