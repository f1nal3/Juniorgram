#include "MessageWidget.hpp"

#include <utility>

MessageWidget::MessageWidget(QString textMessage, QString nameOfUser, QWidget* parent)
    : QWidget(parent),
    messageText(std::move(textMessage)),
    userName(std::move(nameOfUser)),
    timeMessage(QTime::currentTime())
{
    initializationUi();
    uiConnect();
    reactionOnMessage.append(NO_SELECTED);
    updateWidget();
    setLayout(mainLayout);
}

MessageWidget::MessageWidget() 
    : MessageWidget(EMPTY_MESSAGE, EMPTY_USER_NAME){}

MessageWidget::MessageWidget(QString textMessage)
    : MessageWidget(std::move(textMessage), EMPTY_USER_NAME){}


MessageWidget::~MessageWidget() = default;

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
    messageTextEdit = new QTextEdit();
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
    
    deleteButton = new QPushButton("Delete");
    deleteButton->setObjectName(QString::fromUtf8("deleteButton"));

    DownLevelLayout->addWidget(reactionChoseBox);
    DownLevelLayout->addItem(horizontalDownSpacer);
    DownLevelLayout->addWidget(deleteButton);

    //mainLayout
    mainLayout->addLayout(UpLevelLayout);
    mainLayout->addWidget(messageTextEdit);
    mainLayout->addLayout(DownLevelLayout);
}

void MessageWidget::deleteButtonClick()
{
    QWidget::close(); }

bool MessageWidget::isReaction() { return (reactionMap["Like"] > 0) ? false : true; }

QString MessageWidget::getReaction()
{
    return (isReaction()) ? NO_SELECTED_RETURN : reactionOnMessage;
}

void MessageWidget::updateWidget()
{
    messageTimeEdit->setTime(timeMessage);
    messageTextEdit->setText(messageText);
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
