#include "MessageWidget.hpp"
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
    reactionOnMessage.append(NO_SELECTED);
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
    connect(reactionChoseBox, SIGNAL(currentIndexChanged(QString)), SLOT(reactionChange(QString)));
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

    reactionLabel = new QLabel;
    reactionLabel->setText("reaction");

    horizontalUpLeftSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    messageTimeEdit = new QTimeEdit(this);
    messageTimeEdit->setObjectName(QString::fromUtf8("messageTimeEdit"));
    messageTimeEdit->setFrame(false);
    messageTimeEdit->setReadOnly(true);
    messageTimeEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    messageTimeEdit->setKeyboardTracking(true);

    horizontalUpRightSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
    
    userNameLabel = new QLabel;
    userNameLabel->setText("userName");

    UpLevelLayout->addWidget(reactionLabel);
    UpLevelLayout->addItem(horizontalUpLeftSpacer);
    UpLevelLayout->addWidget(userNameLabel);
    UpLevelLayout->addItem(horizontalUpRightSpacer);
    UpLevelLayout->addWidget(messageTimeEdit);

    //DownLevelLayout

    reactionChoseBox = new QComboBox();
    reactionChoseBox->setObjectName(QString::fromUtf8("reactionChoseBox"));
    reactionChoseBox->addItem(QString("Choose"));
    reactionChoseBox->addItem(QString("Like"));

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
        delete deleteButton;
        delete horizontalDownSpacer;

        delete UpLevelLayout;
        delete DownLevelLayout;
    }
}


void MessageWidget::deleteButtonClick()
{
    clearMessage();
    messageItem->setSizeHint(QSize(0, 30));
    delMessage = new QLabel("Message was delete");
    mainLayout->addWidget(delMessage);
    messageDel = true;
}

bool MessageWidget::isReaction() { return (reactionMap["Like"] > 0) ? false : true; }

QString MessageWidget::getReaction()
{
    return (isReaction()) ? NO_SELECTED_RETURN : reactionOnMessage;
}

void MessageWidget::updateWidget()
{
    messageTimeEdit->setTime(timeMessage);
    messageTextEdit->setPlainText(messageText);
    userNameLabel->setText(userName);
    reactionLabel->setText("");
    if (!isReaction())
    {
        reactionOnMessage.clear();
        reactionOnMessage += "Like " + QString::number(reactionMap["Like"]);
        reactionLabel->setText(reactionOnMessage);
    }
}

void MessageWidget::reactionChange(QString newReaction)
{
    if (newReaction != "Choose")
    {
        ++reactionMap[newReaction.toStdString()];
        newReaction += " " + (QString::number(reactionMap[newReaction.toStdString()]));
        reactionOnMessage.clear();
        reactionOnMessage.append(newReaction);
        reactionLabel->setText(reactionOnMessage);
    }
    else
    {
        --reactionMap["Like"];
        if (reactionMap[newReaction.toStdString()] > 0)
        {
            newReaction = "Like";
            newReaction += " " + (QString::number(reactionMap[newReaction.toStdString()]));
            reactionOnMessage.clear();
            reactionOnMessage.append(newReaction);
            reactionLabel->setText(reactionOnMessage);
        }
        else
        {
            reactionOnMessage.clear();
            reactionLabel->setText("");
            reactionLabel->setText(reactionOnMessage);
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

void MessageWidget::setUserName(QString newUserName)
{
    if (newUserName != "")
    {
        userName = newUserName;
        updateWidget();
    }
}
