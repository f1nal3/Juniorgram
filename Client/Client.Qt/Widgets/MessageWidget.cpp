#include "MessageWidget.hpp"
#include "Style/Style.hpp"
#include <utility>

MessageWidget::MessageWidget(QString textMessage, QString nameOfUser, QListWidgetItem* Item,
                             bool deletedMessage, QWidget* parent)
    : QWidget(parent),
      messageText(std::move(textMessage)),
      userName(std::move(nameOfUser)),
      dateTimeMessage(QDateTime::currentDateTime())
{
    messageItem = Item;
    messageDeleted = deletedMessage;
    // Main layouts
    mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
    setLayout(mainLayout);

    if (!deletedMessage)
    {
        initializationUiNotDelete();
        uiConnect();
        updateWidget();
    }
    else
        initializationUiDelete();
}

MessageWidget::MessageWidget(std::string textMessage, std::string nameOfUser, QListWidgetItem* Item,
                             bool deletedMessage)
    : MessageWidget(QString::fromStdString(textMessage), QString::fromStdString(nameOfUser), Item,
                    deletedMessage){}

MessageWidget::MessageWidget(QString textMessage, QListWidgetItem* Item, bool deletedMessage)
    : MessageWidget(textMessage, EMPTY_USER_NAME, Item, deletedMessage){}

MessageWidget::~MessageWidget()
{
    clearMessage();
    delete mainLayout;
}

void MessageWidget::uiConnect()
{
    connect(reactionChoseBox, SIGNAL(currentIndexChanged(int)), SLOT(reactionChange(int)));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteButtonClick()));
}

void MessageWidget::initializationUiDelete() {
    messageItem->setSizeHint(QSize(0, Style::valueDPIScale(40)));
    delMessage = new Label("Message was deleted");
    delMessage->setFont(QFont("Noto Sans", Style::valueDPIScale(12)));
    mainLayout->addWidget(delMessage);
}

void MessageWidget::initializationUiNotDelete()
{
    // Main Layouts
    UpLevelLayout = new QHBoxLayout();
    UpLevelLayout->setObjectName(QString::fromUtf8("UpLevelLayout"));

    DownLevelLayout = new QHBoxLayout();
    DownLevelLayout->setObjectName(QString::fromUtf8("DownLevelLayout"));

    // message
    messageTextEdit = new FlatPlainTextEdit();
    messageTextEdit->setObjectName(QString::fromUtf8("message"));
    messageTextEdit->setEnabled(true);
    messageTextEdit->setTabletTracking(false);
    messageTextEdit->setFocusPolicy(Qt::TabFocus);
    messageTextEdit->setAcceptDrops(true);
    messageTextEdit->setFrameShape(QFrame::WinPanel);
    messageTextEdit->setLineWidth(1);
    messageTextEdit->setReadOnly(true);

    // UpLevelLayout

    reactionLabel = new Label;
    reactionLabel->setText("");

    reactionLabelIcon = new Label;
    reactionLabelIcon->setText("");
    LikeIcon = new QPixmap(":/reactions/like.png");

    horizontalUpLeftSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    messageDateTimeEdit = new DateTimeEdit(this);
    messageDateTimeEdit->setObjectName(QString::fromUtf8("messageDateTimeEdit"));

    horizontalUpRightSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    userNameLabel = new Label;
    userNameLabel->setText("userName");

    UpLevelLayout->addWidget(reactionLabelIcon);
    UpLevelLayout->addWidget(reactionLabel);
    UpLevelLayout->addItem(horizontalUpLeftSpacer);
    UpLevelLayout->addWidget(userNameLabel);
    UpLevelLayout->addItem(horizontalUpRightSpacer);
    UpLevelLayout->addWidget(messageDateTimeEdit);

    // DownLevelLayout

    reactionChoseBox = new ComboBox();
    reactionChoseBox->setObjectName(QString::fromUtf8("reactionChoseBox"));
    reactionChoseBox->addItem(QIcon(":/reactions/smile.png"), "");
    reactionChoseBox->addItem(QIcon(":/reactions/like.png"), "");
    reactionChoseBox->setMinimumWidth(Style::valueDPIScale(45));

    horizontalDownSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    deleteButton = new FlatButton("Delete");
    deleteButton->setObjectName(QString::fromUtf8("deleteButton"));

    DownLevelLayout->addWidget(reactionChoseBox);
    DownLevelLayout->addItem(horizontalDownSpacer);
    DownLevelLayout->addWidget(deleteButton);

    // mainLayout
    mainLayout->addLayout(UpLevelLayout);
    mainLayout->addWidget(messageTextEdit);
    mainLayout->addLayout(DownLevelLayout);
}

void MessageWidget::clearMessage()
{
    if (messageDeleted)
    {
        mainLayout->removeWidget(delMessage);
        delete delMessage;
    }
    else
    {
        UpLevelLayout->removeWidget(reactionLabel);
        UpLevelLayout->removeItem(horizontalUpLeftSpacer);
        UpLevelLayout->removeWidget(userNameLabel);
        UpLevelLayout->removeItem(horizontalUpRightSpacer);
        UpLevelLayout->removeWidget(messageDateTimeEdit);
        UpLevelLayout->removeWidget(reactionLabelIcon);
        DownLevelLayout->removeWidget(reactionChoseBox);
        DownLevelLayout->removeItem(horizontalDownSpacer);
        DownLevelLayout->removeWidget(deleteButton);
        delete messageTextEdit;

        delete userNameLabel;
        delete reactionLabel;
        delete horizontalUpLeftSpacer;
        delete horizontalUpRightSpacer;
        delete messageDateTimeEdit;
        delete reactionChoseBox;
        delete reactionLabelIcon;
        delete deleteButton;
        delete horizontalDownSpacer;

        delete UpLevelLayout;
        delete DownLevelLayout;
    }
}


void MessageWidget::deleteButtonClick()
{
    clearMessage();
    messageItem->setSizeHint(QSize(0, Style::valueDPIScale(40)));
    delMessage = new Label("Message was deleted");
    delMessage->setFont(QFont("Noto Sans", Style::valueDPIScale(12)));
    mainLayout->addWidget(delMessage);
    messageDeleted = true;
}

bool MessageWidget::isReaction(QString reaction)
{
    return (reactionMap[reaction.toStdString()] > 0) ? false : true;
}

void MessageWidget::updateWidget()
{
    messageDateTimeEdit->setDateTime(dateTimeMessage);
    messageTextEdit->setPlainText(messageText);
    userNameLabel->setText(userName);
    reactionLabel->setText("");
    if (!isReaction("Like"))
    {
        reactionOnMessage.clear();
        reactionOnMessage = QString::number(reactionMap["Like"]);
        reactionLabel->setText(reactionOnMessage);
        reactionLabelIcon->setPixmap(LikeIcon[0]);
    }
}

void MessageWidget::reactionChange(int index)
{
    switch (index)
    {
        case 1:
        {
            ++reactionMap["Like"];
            reactionLabelIcon->setPixmap(LikeIcon[0].scaled(
                QSize(Style::valueDPIScale(16), Style::valueDPIScale(16)), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            reactionLabel->setText(QString::number(reactionMap["Like"]));
            break;
        }
        default:
        {
            --reactionMap["Like"];
            if (reactionMap["Like"] < 1)
            {
                reactionLabelIcon->clear();
                reactionLabel->clear();
            }
            else
            {
                reactionLabel->setText(QString::number(reactionMap["Like"]));
            }
            break;
        }
    }
}

void MessageWidget::setThisItem(QListWidgetItem* Item) { messageItem = Item; }

void MessageWidget::setMessageText(QString newMessage)
{
    if (newMessage != "")
    {
        messageText = newMessage;
        updateWidget();
    }
}

void MessageWidget::setStdMessageText(std::string newMessage)
{
    if (newMessage != "")
    {
        messageText = QString::fromStdString(newMessage);
        updateWidget();
    }
}

void MessageWidget::setUserName(QString newUserName)
{
    if (newUserName != "")
    {
        userName = newUserName;
        updateWidget();
    }
}

void MessageWidget::setStdUserName(std::string newUserName)
{
    if (newUserName != "")
    {
        userName = QString::fromStdString(newUserName);
        updateWidget();
    }
}
