#include "MessageWidget.hpp"

#include <utility>

#include "Style/Style.hpp"

MessageWidget::MessageWidget(QString textMessage, QString nameOfUser, QListWidgetItem* Item,
                             bool deletedMessage, QWidget* parent)
    : QWidget(parent),
      messageText(std::move(textMessage)),
      userName(std::move(nameOfUser)),
      dateTimeMessage(QDateTime::currentDateTime())
{
    reactionUserOnMessage = reactions::Non;
    messageItem    = Item;
    messageDeleted = deletedMessage;
    // Main layouts
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

MessageWidget::MessageWidget(std::string textMessage, std::string nameOfUser, QListWidgetItem* Item,
                             bool deletedMessage)
    : MessageWidget(QString::fromStdString(textMessage), QString::fromStdString(nameOfUser), Item,
                    deletedMessage)
{
}

MessageWidget::MessageWidget(QString textMessage, QListWidgetItem* Item, bool deletedMessage)
    : MessageWidget(textMessage, EMPTY_USER_NAME, Item, deletedMessage)
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

    reactionMapLabel = new QMap<reactions, Label*>;

    reactionMapLabel->insert(reactions::like, new Label);
    reactionMapLabel->insert(reactions::dislike, new Label);
    reactionMapLabel->insert(reactions::fire, new Label);
    reactionMapLabel->insert(reactions::cat, new Label);

    reactionLabelIconLike = new Label;
    reactionLabelIconDislike = new Label;
    reactionLabelIconFire = new Label;
    reactionLabelIconCat = new Label;

    pixmapIcon = new QMap<reactions, QPixmap*>;

    pixmapIcon->insert(reactions::like, new QPixmap(":/reactions/like.png"));
    pixmapIcon->insert(reactions::dislike, new QPixmap(":/reactions/dislike.png"));
    pixmapIcon->insert(reactions::fire, new QPixmap(":/reactions/fire.png"));
    pixmapIcon->insert(reactions::cat, new QPixmap(":/reactions/cat.png"));

    horizontalUpLeftSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    messageDateTimeEdit = new DateTimeEdit(this);

    horizontalUpRightSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    userNameLabel = new Label;
    userNameLabel->setText("userName");

    UpLevelLayout->addWidget(reactionLabelIconLike);
    UpLevelLayout->addWidget(*reactionMapLabel->find(reactions::like));
    UpLevelLayout->addWidget(reactionLabelIconDislike);
    UpLevelLayout->addWidget(*reactionMapLabel->find(reactions::dislike));
    UpLevelLayout->addWidget(reactionLabelIconFire);
    UpLevelLayout->addWidget(*reactionMapLabel->find(reactions::fire));
    UpLevelLayout->addWidget(reactionLabelIconCat);
    UpLevelLayout->addWidget(*reactionMapLabel->find(reactions::cat));

    UpLevelLayout->addItem(horizontalUpLeftSpacer);
    UpLevelLayout->addWidget(userNameLabel);
    UpLevelLayout->addItem(horizontalUpRightSpacer);
    UpLevelLayout->addWidget(messageDateTimeEdit);

    // DownLevelLayout

    reactionChoseBox = new ComboBox();
    reactionChoseBox->addItem(QIcon(":/reactions/smile.png"), "");
    reactionChoseBox->addItem(QIcon(":/reactions/like.png"), "");
    reactionChoseBox->addItem(QIcon(":/reactions/dislike.png"), "");
    reactionChoseBox->addItem(QIcon(":/reactions/fire.png"), "");
    reactionChoseBox->addItem(QIcon(":/reactions/cat.png"), "");
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
        //UpLevelLayout->removeWidget(reactionLabel);
        UpLevelLayout->removeItem(horizontalUpLeftSpacer);
        UpLevelLayout->removeWidget(userNameLabel);
        UpLevelLayout->removeItem(horizontalUpRightSpacer);
        UpLevelLayout->removeWidget(messageDateTimeEdit);
        UpLevelLayout->removeWidget(reactionLabelIconLike);
        UpLevelLayout->removeWidget(reactionLabelIconDislike);
        UpLevelLayout->removeWidget(reactionLabelIconFire);
        UpLevelLayout->removeWidget(reactionLabelIconCat);
        DownLevelLayout->removeWidget(reactionChoseBox);
        DownLevelLayout->removeItem(horizontalDownSpacer);
        DownLevelLayout->removeWidget(deleteButton);
        delete messageTextEdit;

        delete userNameLabel;
        //delete reactionLabel;
        delete horizontalUpLeftSpacer;
        delete horizontalUpRightSpacer;
        delete messageDateTimeEdit;
        delete reactionChoseBox;
        delete reactionLabelIconLike;
        delete reactionLabelIconDislike;
        delete reactionLabelIconFire;
        delete reactionLabelIconCat;
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

//bool MessageWidget::isReaction(QString reaction)
//{
//    return (reactionMap[reaction.toStdString()] > 0) ? false : true;
//}

void MessageWidget::updateWidget()
{
    messageDateTimeEdit->setDateTime(dateTimeMessage);
    messageTextEdit->setText(messageText);
    userNameLabel->setText(userName);
}

void MessageWidget::reactionChange(int index)
{
    if (reactionUserOnMessage != reactions::Non)
    {
        --reactionMap[reactionUserOnMessage];
        if (reactionMap[reactions::like] <= 0)
        {
            reactionLabelIconLike->clear();
            reactionMapLabel->find(reactions::like).value()->clear();
        }
        if (reactionMap[reactions::dislike] <= 0)
        {
            reactionLabelIconDislike->clear();
            reactionMapLabel->find(reactions::dislike).value()->clear();
        }
        if (reactionMap[reactions::fire] <= 0)
        {
            reactionLabelIconFire->clear();
            reactionMapLabel->find(reactions::fire).value()->clear();
        }
        if (reactionMap[reactions::cat] <= 0)
        {
            reactionLabelIconCat->clear();
            reactionMapLabel->find(reactions::cat).value()->clear();
        }
    }
    switch (index)
    {
        case 1:
        {
            reactionLabelIconLike->setPixmap(
                pixmapIcon->find(reactions::like)
                    .value()[0]
                    .scaled(
                QSize(Style::valueDPIScale(16), Style::valueDPIScale(16)),
                                   Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ++reactionMap[reactions::like];
            reactionMapLabel->find(reactions::like)
                .value()
                ->setText(QString::number(reactionMap[reactions::like]));
            reactionUserOnMessage = reactions::like;
            break;
        }
        case 2:
        {
            reactionLabelIconDislike->setPixmap(
                pixmapIcon->find(reactions::dislike)
                    .value()[0]
                    .scaled(QSize(Style::valueDPIScale(16), Style::valueDPIScale(16)),
                            Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ++reactionMap[reactions::dislike];
            reactionMapLabel->find(reactions::dislike)
                .value()
                ->setText(QString::number(reactionMap[reactions::dislike]));
            reactionUserOnMessage = reactions::dislike;
            break;
        }
        case 3:
        {
            reactionLabelIconFire->setPixmap(
                pixmapIcon->find(reactions::fire)
                    .value()[0]
                    .scaled(QSize(Style::valueDPIScale(16), Style::valueDPIScale(16)),
                            Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ++reactionMap[reactions::fire];
            reactionMapLabel->find(reactions::fire)
                .value()
                ->setText(QString::number(reactionMap[reactions::fire]));
            reactionUserOnMessage = reactions::fire;
            break;
        }
        case 4:
        {
            reactionLabelIconCat->setPixmap(
                pixmapIcon->find(reactions::cat)
                    .value()[0]
                    .scaled(QSize(Style::valueDPIScale(16), Style::valueDPIScale(16)),
                            Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ++reactionMap[reactions::cat];
            reactionMapLabel->find(reactions::cat)
                .value()
                ->setText(QString::number(reactionMap[reactions::cat]));
            reactionUserOnMessage = reactions::cat;
            break;
        }
        default:
        {
            reactionUserOnMessage = reactions::Non;
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
    //reactionMap = std::move(newReactionMap);
    updateWidget();
}
