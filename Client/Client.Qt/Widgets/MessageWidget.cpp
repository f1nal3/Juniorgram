#include "MessageWidget.hpp"
#include "Style/Style.hpp"
#include <utility>

MessageWidget::MessageWidget(QString textMessage, QString nameOfUser,
                             QListWidgetItem* Item, QWidget* parent)
    : QWidget(parent),
    messageText(std::move(textMessage)),
    userName(std::move(nameOfUser)),
    timeMessage(QTime::currentTime())
{
    initializationUi();
    uiConnect();
    messageItem = Item;
    messageDel = false;
    updateWidget();
    setLayout(mainLayout);
}

MessageWidget::MessageWidget(QString textMessage, QListWidgetItem* Item)
    : MessageWidget(textMessage, EMPTY_USER_NAME, Item){}

MessageWidget::MessageWidget(QListWidgetItem* Item)
    : MessageWidget(EMPTY_MESSAGE, EMPTY_USER_NAME, Item){}

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

void MessageWidget::initializationUi()
{
    //Layouts
    mainLayout    = new QVBoxLayout(this);
    mainLayout->setObjectName(QString::fromUtf8("mainLayout"));

    UpLevelLayout = new QHBoxLayout();
    UpLevelLayout->setObjectName(QString::fromUtf8("UpLevelLayout"));

    DownLevelLayout = new QHBoxLayout();
    DownLevelLayout->setObjectName(QString::fromUtf8("DownLevelLayout"));

    //message
    messageTextEdit = new FlatPlainTextEdit();
    messageTextEdit->setObjectName(QString::fromUtf8("message"));
    messageTextEdit->setEnabled(true);
    messageTextEdit->setTabletTracking(false);
    messageTextEdit->setFocusPolicy(Qt::TabFocus);
    messageTextEdit->setAcceptDrops(true);
    messageTextEdit->setFrameShape(QFrame::WinPanel);
    messageTextEdit->setLineWidth(1);
    messageTextEdit->setReadOnly(true);

    //UpLevelLayout

    reactionLabel = new Label;
    reactionLabel->setText("");

    reactionLabelIcon = new Label;
    reactionLabelIcon->setText("");
    LikeIcon = new QPixmap(":/reactions/like.png");

    horizontalUpLeftSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    messageTimeEdit = new TimeEdit(this);
    messageTimeEdit->setObjectName(QString::fromUtf8("messageTimeEdit"));

    horizontalUpRightSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
    
    userNameLabel = new Label;
    userNameLabel->setText("userName");

    UpLevelLayout->addWidget(reactionLabelIcon);
    UpLevelLayout->addWidget(reactionLabel);
    UpLevelLayout->addItem(horizontalUpLeftSpacer);
    UpLevelLayout->addWidget(userNameLabel);
    UpLevelLayout->addItem(horizontalUpRightSpacer);
    UpLevelLayout->addWidget(messageTimeEdit);

    //DownLevelLayout

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

    //mainLayout
    mainLayout->addLayout(UpLevelLayout);
    mainLayout->addWidget(messageTextEdit);
    mainLayout->addLayout(DownLevelLayout);
}

void MessageWidget::clearMessage()
{
    if (messageDel)
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
        UpLevelLayout->removeWidget(messageTimeEdit);
        UpLevelLayout->removeWidget(reactionLabelIcon);
        DownLevelLayout->removeWidget(reactionChoseBox);
        DownLevelLayout->removeItem(horizontalDownSpacer);
        DownLevelLayout->removeWidget(deleteButton);
        delete messageTextEdit;

        delete userNameLabel;
        delete reactionLabel;
        delete horizontalUpLeftSpacer;
        delete horizontalUpRightSpacer;
        delete messageTimeEdit;
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
    messageDel = true;
}

bool MessageWidget::isReaction(QString reaction)
{
    return (reactionMap[reaction.toStdString()] > 0) ? false : true;
}

void MessageWidget::updateWidget()
{
    messageTimeEdit->setTime(timeMessage);
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
