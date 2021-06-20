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
    messageTextEdit        = new FlatPlainTextEdit;
    sendButton      = new FlatButton(this, "Send");
    sendButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    textEdit               = new TextEdit(messageTextEdit);
    horizontalButtonSpacer = new QSpacerItem(40, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(channelListWidget, 20);
    mainLayout->addLayout(rightLayout, 90);
    rightLayout->addWidget(chatWidget, 85);
    rightLayout->addLayout(messageLayout, 15);
    messageLayout->addWidget(messageTextEdit);
    messageLayout->addLayout(messageButtonLayout);
    messageButtonLayout->addWidget(textEdit);
    messageButtonLayout->addItem(horizontalButtonSpacer);
    messageButtonLayout->addWidget(sendButton);
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
    sendButton->setClickCallback([&]() { updateMessagesList_User(); });
    connect(chatWidget, SIGNAL(itemClicked(QListWidgetItem*)), this,
            SLOT(deletingSelection(QListWidgetItem*)));
    connect(chatWidget->model(), SIGNAL(rowsInserted(QModelIndex, int, int)), chatWidget,
            SLOT(scrollToBottom()));
}

void ChatWindow::deletingSelection(QListWidgetItem* item) { item->setSelected(false); }

void ChatWindow::updateMessagesList_User()
{
    if (textEdit->text() != "")
        newMessage(textEdit->text());
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
    delete sendButton;
    delete messageTextEdit;
    delete textEdit;
    delete chatWidget;
    delete messageLayout;
    delete rightLayout;
    delete mainLayout;
}
