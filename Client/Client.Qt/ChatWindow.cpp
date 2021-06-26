#include "ChatWindow.hpp"

#include <utility>

#include "Style/Style.hpp"

ChatWindow::ChatWindow(QWidget* parent) : QWidget(parent)
{
    setContentsMargins(0, 0, 0, 0);
    mainLayout          = new QHBoxLayout(this);
    rightLayout         = new QVBoxLayout();
    messageLayout       = new QVBoxLayout();
    messageButtonLayout = new QHBoxLayout();
    channelListWidget   = new ChannelListWidget();
    chatWidget          = new QListWidget();
    chatWidget->setLayoutMode(QListView::Batched);
    chatWidget->setMovement(QListView::Free);
    chatWidget->setStyleSheet(
        QString("QListWidget{ "
                "border: 0px;"
                "background: #323232;"
                "}"));
    chatWidget->setDragEnabled(false);
    textEdit               = new TextEdit(&messageTextEdit, &sendButton);
    horizontalButtonSpacer = new QSpacerItem(40, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(channelListWidget, 20);
    mainLayout->addLayout(rightLayout, 90);
    rightLayout->addWidget(chatWidget, 85);
    rightLayout->addLayout(messageLayout, 15);
    messageLayout->addLayout(messageButtonLayout);
    messageButtonLayout->addWidget(textEdit);
    connectUi();
    setLayout(mainLayout);
}

void ChatWindow::keyPressEvent(QKeyEvent* event)
{
    if ((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) &&
        (textEdit->getText() != ""))
    {
        newMessage(textEdit->getText());
        textEdit->clear();
    }
}

void ChatWindow::connectUi()
{
    sendButton->setClickCallback([&]() { updateMessagesList_User(); });
    connect(chatWidget, SIGNAL(itemClicked(QListWidgetItem*)), this,
            SLOT(deletingSelection(QListWidgetItem*)));
    connect(chatWidget->model(), SIGNAL(rowsInserted(QModelIndex, int, int)), chatWidget,
            SLOT(scrollToBottom()));
}

void ChatWindow::deletingSelection(QListWidgetItem* item) { item->setSelected(false); }

void ChatWindow::updateMessagesList_User()
{
    if (textEdit->getText() != "")
        newMessage(textEdit->getText());
    textEdit->clear();
}

void ChatWindow::newMessage(QString textMessage, QString userNameMessage)
{
    auto* item = new QListWidgetItem();
    item->setSizeHint(QSize(0, Style::valueDPIScale(150)));
    auto* myItem =
        new MessageWidget(std::move(textMessage), std::move(userNameMessage), item, false);
    myItem->setThisItem(item);
    chatWidget->addItem(item);
    chatWidget->setItemWidget(item, myItem);
}

void ChatWindow::newMessage(QString textMessage)
{
    auto* item = new QListWidgetItem();
    item->setSizeHint(QSize(0, Style::valueDPIScale(150)));
    auto* myItem = new MessageWidget(std::move(textMessage), item, false);
    myItem->setThisItem(item);
    chatWidget->addItem(item);
    chatWidget->setItemWidget(item, myItem);
}

ChatWindow::~ChatWindow()
{
    delete messageButtonLayout;
    delete channelListWidget;
    delete messageTextEdit;
    delete textEdit;
    delete chatWidget;
    delete messageLayout;
    delete rightLayout;
    delete mainLayout;
}
