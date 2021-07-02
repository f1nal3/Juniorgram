#include "ChatWidget.hpp"

ChatWidget::ChatWidget(QWidget* parent)
    : QWidget(parent)
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
    textEdit = new TextEdit(chatWidget);

    mainChatLayout->addWidget(chatWidget, 85);
    mainChatLayout->addWidget(textEdit, 15);
    setLayout(mainChatLayout);
    connectUi();
}

void ChatWidget::newMessage(QString messageText)
{
    auto* item = new QListWidgetItem();
    item->setSizeHint(QSize(0, Style::valueDPIScale(150)));
    auto* myItem = new MessageWidget(std::move(messageText), item, false);
    myItem->setThisItem(item);
    chatWidget->addItem(item);
    chatWidget->setItemWidget(item, myItem);
}

 void ChatWidget::newMessage(QString messageText, QString userNameMessage)
{
    auto* item = new QListWidgetItem();
    item->setSizeHint(QSize(0, Style::valueDPIScale(150)));
    auto* myItem =
        new MessageWidget(std::move(messageText), std::move(userNameMessage), item, false);
    myItem->setThisItem(item);
    chatWidget->addItem(item);
    chatWidget->setItemWidget(item, myItem);
}

void ChatWidget::connectUi()
{
    connect(chatWidget, SIGNAL(itemClicked(QListWidgetItem*)), this,
            SLOT(deletingSelection(QListWidgetItem*)));
    connect(chatWidget->model(), SIGNAL(rowsInserted(QModelIndex, int, int)), chatWidget,
            SLOT(scrollToBottom()));
    connect(textEdit, SIGNAL(sendMessageSignal(QString)), this, SLOT(newMessage(QString)));
}

void ChatWidget::deletingSelection(QListWidgetItem* item) { item->setSelected(false); }

ChatWidget::~ChatWidget()
{
    delete mainChatLayout;
    delete chatWidget;
    delete textEdit;
}
