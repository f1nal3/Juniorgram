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

ChatWidget::~ChatWidget() 
{ 
    delete mainChatLayout;
    delete chatWidget;
    delete textEdit;
}

void ChatWidget::connectUi()
{
    connect(chatWidget, SIGNAL(itemClicked(QListWidgetItem*)), this,
            SLOT(deletingSelection(QListWidgetItem*)));
    connect(chatWidget->model(), SIGNAL(rowsInserted(QModelIndex, int, int)), chatWidget,
            SLOT(scrollToBottom()));
}

void ChatWidget::deletingSelection(QListWidgetItem* item) { item->setSelected(false); }
