#include "MessageWidget.hpp"

#include <utility>

#include "Style/Style.hpp"

MessageWidget::MessageWidget(QString textMessage, QString nameOfUser, QListWidgetItem* item,
                             bool deletedMessage, QWidget* parent)
    : QWidget(parent),
      messageText(std::move(textMessage)),
      userName(std::move(nameOfUser)),
      dateTimeMessage(QDateTime::currentDateTime())
{
    messageItem    = item;
    messageDeleted = deletedMessage;
    mainLayout = new QVBoxLayout(this);
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

MessageWidget::MessageWidget(std::string textMessage, std::string nameOfUser, QListWidgetItem* item,
                             bool deletedMessage)
    : MessageWidget(QString::fromStdString(textMessage), QString::fromStdString(nameOfUser), item,
                    deletedMessage)
{
}

MessageWidget::MessageWidget(QString textMessage, QListWidgetItem* item, bool deletedMessage)
    : MessageWidget(textMessage, EMPTY_USER_NAME, item, deletedMessage)
{
}

MessageWidget::~MessageWidget()
{
    clearMessage();
    delete mainLayout;
}

void MessageWidget::uiConnect()
{
    connect(reactionChoseBox, SIGNAL(currentIndexChanged(int)), SLOT(reactionChange(int)));
    deleteButton->setClickCallback([&]() { deleteButtonClick(); });
}

void MessageWidget::initializationUiDelete()
{
    messageItem->setSizeHint(QSize(0, Style::valueDPIScale(40)));
    delMessage = new Label("Message was deleted");
    delMessage->setFont(st::semiboldFont);
    mainLayout->addWidget(delMessage);
}

void MessageWidget::initializationUiNotDelete()
{
    // Main Layouts
    UpLevelLayout   = new QHBoxLayout();
    DownLevelLayout = new QHBoxLayout();

    // message
    messageTextEdit = new FlatPlainTextEdit();
    messageTextEdit->setTabletTracking(false);
    messageTextEdit->setAcceptDrops(false);
    messageTextEdit->setReadOnly(true);

    reactionLabel = new Label;
    reactionLabel->setText("");

    reactionLabelIcon = new Label;
    reactionLabelIcon->setText("");
    LikeIcon = new QPixmap(":/reactions/like.png");

    horizontalUpLeftSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    messageDateTimeEdit = new DateTimeEdit(this);

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
    reactionChoseBox->addItem(QIcon(":/reactions/smile.png"), "");
    reactionChoseBox->addItem(QIcon(":/reactions/like.png"), "");
    reactionChoseBox->setMinimumWidth(Style::valueDPIScale(45));

    horizontalDownSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    deleteButton = new FlatButton(this, "Delete");

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
    messageTextEdit->setText(messageText);
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
            reactionLabelIcon->setPixmap(
                LikeIcon[0].scaled(fontInfo().pixelSize(), fontInfo().pixelSize(),
                                   Qt::KeepAspectRatio, Qt::SmoothTransformation));
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

void MessageWidget::setThisItem(QListWidgetItem* item) { messageItem = item; }

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

void MessageWidget::setDateTime(QDateTime newDataTime)
{
    dateTimeMessage = std::move(newDataTime);
    updateWidget();
}

void MessageWidget::setStdTime_tDateTime(std::time_t newDataTime)
{
    dateTimeMessage = QDateTime::fromTime_t(newDataTime);
    updateWidget();
}

void MessageWidget::setReactionMap(std::map<std::string, int> newReactionMap)
{
    reactionMap = std::move(newReactionMap);
    updateWidget();
}
