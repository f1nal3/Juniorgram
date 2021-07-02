#include "MessageWidget.hpp"

#include "Style/Style.hpp"

MessageWidget::MessageWidget(QString textMessage, QString nameOfUser, QListWidgetItem* Item,
                             bool deletedMessage, QWidget* parent)
    : QWidget(parent),
      messageText(std::move(textMessage)),
      userName(std::move(nameOfUser)),
      dateTimeMessage(QDateTime::currentDateTime())
{
    reactionUserOnMessage = reactions::NON;
    messageItem           = Item;
    messageDeleted        = deletedMessage;
    // Main layouts
    mainLayout = std::make_unique<QVBoxLayout>(this);
    setLayout(mainLayout.get());
    if (!deletedMessage)
    {
        initializationUiNotDelete();
        connectUi();
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
}

void MessageWidget::connectUi()
{
    connect(reactionChoseBox.get(), SIGNAL(currentIndexChanged(int)), SLOT(reactionChange(int)));
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
    UpLevelLayout   = std::make_unique<QHBoxLayout>();
    DownLevelLayout = std::make_unique<QHBoxLayout>();

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
    pixmapIcon->insert(reactions::LIKE, new QPixmap(":/reactions/like.png"));
    pixmapIcon->insert(reactions::DISLIKE, new QPixmap(":/reactions/dislike.png"));
    pixmapIcon->insert(reactions::FIRE, new QPixmap(":/reactions/fire.png"));
    pixmapIcon->insert(reactions::CAT, new QPixmap(":/reactions/cat.png"));

    horizontalUpLeftSpacer  = std::make_unique<QSpacerItem>(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    messageDateTimeEdit     = new DateTimeEdit(this);
    horizontalUpRightSpacer = std::make_unique<QSpacerItem>(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
    userNameLabel           = new Label;
    userNameLabel->setText("userName");
    for (int i = 0; i < COUNT_REACTION; i++)
    {
        UpLevelLayout->addWidget(*reactionMapLabelIcon->find(i));
        UpLevelLayout->addWidget(*reactionMapLabel->find(i));
    }
    UpLevelLayout->addItem(horizontalUpLeftSpacer.get());
    UpLevelLayout->addWidget(userNameLabel);
    UpLevelLayout->addItem(horizontalUpRightSpacer.get());
    UpLevelLayout->addWidget(messageDateTimeEdit);

    // DownLevelLayout

    reactionChoseBox = std::make_unique<ComboBox>();
    reactionChoseBox->addItem(QIcon(":/reactions/smile.png"), "");
    reactionChoseBox->addItem(QIcon(":/reactions/like.png"), "");
    reactionChoseBox->addItem(QIcon(":/reactions/dislike.png"), "");
    reactionChoseBox->addItem(QIcon(":/reactions/fire.png"), "");
    reactionChoseBox->addItem(QIcon(":/reactions/cat.png"), "");
    reactionChoseBox->setMinimumWidth(Style::valueDPIScale(45));
#ifdef Q_OS_MAC
    reactionChoseBox->setMinimumWidth(Style::valueDPIScale(65));
#endif

    horizontalDownSpacer =
        std::make_unique <QSpacerItem>(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    deleteButton = std::make_unique<FlatButton>(this, "Delete");

    DownLevelLayout->addWidget(reactionChoseBox.get());
    DownLevelLayout->addItem(horizontalDownSpacer.get());
    DownLevelLayout->addWidget(deleteButton.get());

    // mainLayout
    mainLayout->addLayout(UpLevelLayout.get());
    mainLayout->addWidget(messageTextEdit);
    mainLayout->addLayout(DownLevelLayout.get());
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
        UpLevelLayout->removeItem(horizontalUpLeftSpacer.get());
        UpLevelLayout->removeWidget(userNameLabel);
        UpLevelLayout->removeItem(horizontalUpRightSpacer.get());
        UpLevelLayout->removeWidget(messageDateTimeEdit);
        UpLevelLayout->removeWidget(messageTextEdit);
        for (int i = 0; i < COUNT_REACTION; i++)
        {
            UpLevelLayout->removeWidget(*reactionMapLabel->find(i));
            UpLevelLayout->removeWidget(*reactionMapLabelIcon->find(i));
            delete *reactionMapLabel->find(i);
            delete *reactionMapLabelIcon->find(i);
        }
        DownLevelLayout->removeWidget(reactionChoseBox.get());
        DownLevelLayout->removeItem(horizontalDownSpacer.get());
        DownLevelLayout->removeWidget(deleteButton.get());
        mainLayout->removeItem(UpLevelLayout.get());
        delete messageTextEdit;
        delete userNameLabel;
        delete reactionMapLabel;
        delete reactionMapLabelIcon;
        delete messageDateTimeEdit;
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
