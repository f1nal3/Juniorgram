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
    reactionUserOnMessage = reactions::NON;
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

    reactionMapLabel     = new QMap<int, Label*>;
    reactionMapLabelIcon = new QMap<int, Label*>;
    pixmapIcon           = new QMap<int, QPixmap*>;
    for (int i = 0; i < COUNT_REACTION; i++)
    {
        reactionMapLabel->insert(i, new Label);
        reactionMapLabelIcon->insert(i, new Label);
    }
    pixmapIcon->insert(reactions::LIKE, new QPixmap(":/reactions/LIKE.png"));
    pixmapIcon->insert(reactions::DISLIKE, new QPixmap(":/reactions/DISLIKE.png"));
    pixmapIcon->insert(reactions::FIRE, new QPixmap(":/reactions/FIRE.png"));
    pixmapIcon->insert(reactions::CAT, new QPixmap(":/reactions/CAT.png"));

    horizontalUpLeftSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    messageDateTimeEdit = new DateTimeEdit(this);
    horizontalUpRightSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
    userNameLabel = new Label;
    userNameLabel->setText("userName");
    for (int i = 0; i < COUNT_REACTION; i++)
    {
        UpLevelLayout->addWidget(*reactionMapLabelIcon->find(i));
        UpLevelLayout->addWidget(*reactionMapLabel->find(i));
    }
    UpLevelLayout->addItem(horizontalUpLeftSpacer);
    UpLevelLayout->addWidget(userNameLabel);
    UpLevelLayout->addItem(horizontalUpRightSpacer);
    UpLevelLayout->addWidget(messageDateTimeEdit);

    // DownLevelLayout

    reactionChoseBox = new ComboBox();
    reactionChoseBox->addItem(QIcon(":/reactions/smile.png"), "");
    reactionChoseBox->addItem(QIcon(":/reactions/LIKE.png"), "");
    reactionChoseBox->addItem(QIcon(":/reactions/DISLIKE.png"), "");
    reactionChoseBox->addItem(QIcon(":/reactions/FIRE.png"), "");
    reactionChoseBox->addItem(QIcon(":/reactions/CAT.png"), "");
    reactionChoseBox->setMinimumWidth(Style::valueDPIScale(45));
    #ifdef Q_OS_MAC
        reactionChoseBox->setMinimumWidth(Style::valueDPIScale(65));
    #endif

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
        UpLevelLayout->removeItem(horizontalUpLeftSpacer);
        UpLevelLayout->removeWidget(userNameLabel);
        UpLevelLayout->removeItem(horizontalUpRightSpacer);
        UpLevelLayout->removeWidget(messageDateTimeEdit);
        UpLevelLayout->removeWidget(messageTextEdit);
        for (int i = 0; i < COUNT_REACTION; i++)
        {
            UpLevelLayout->removeWidget(*reactionMapLabel->find(i));
            UpLevelLayout->removeWidget(*reactionMapLabelIcon->find(i));
            delete *reactionMapLabel->find(i);
            delete *reactionMapLabelIcon->find(i);
        }
        DownLevelLayout->removeWidget(reactionChoseBox);
        DownLevelLayout->removeItem(horizontalDownSpacer);
        DownLevelLayout->removeWidget(deleteButton);
        delete messageTextEdit;
        delete userNameLabel;
        delete reactionMapLabel;
        delete reactionMapLabelIcon;
        delete horizontalUpLeftSpacer;
        delete horizontalUpRightSpacer;
        delete messageDateTimeEdit;
        delete reactionChoseBox;
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

void MessageWidget::updateWidget()
{
    messageDateTimeEdit->setDateTime(dateTimeMessage);
    messageTextEdit->setText(messageText);
    userNameLabel->setText(userName);
}

void MessageWidget::reactionChange(int index)
{
    if (reactionUserOnMessage != reactions::NON)
    {
        --reactionMap[reactionUserOnMessage];
        for (auto countReaction : reactionMap)
        {
            if (countReaction.second <= 0)
            {
                reactionMapLabelIcon->find(countReaction.first).value()->clear();
                reactionMapLabel->find(countReaction.first).value()->clear();
            }
        }
    }
    auto reactionSelection{[&](int reactionNumber) {
        reactionMapLabelIcon->find(--reactionNumber)
            .value()
            ->setPixmap(pixmapIcon->find(reactionNumber)
                            .value()[0]
                            .scaled(QSize(Style::valueDPIScale(16), Style::valueDPIScale(16)),
                                    Qt::KeepAspectRatio, Qt::SmoothTransformation));
        reactionMapLabel->find(reactionNumber)
            .value()
            ->setText(QString::number(++reactionMap[reactionNumber]));
        reactionUserOnMessage = static_cast<reactions>(reactionNumber);
    }};
    switch (index)
    {
        case 1:
        case 2:
        case 3:
        case 4:
        {
            reactionSelection(index);
        }
        break;
        default:
        {
            reactionUserOnMessage = reactions::NON;
        }
        break;
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

void MessageWidget::setReactionMap(std::map<int, int> newReactionMap)
{
    reactionMap = std::move(newReactionMap);
    updateWidget();
}
