#include "qmessagewidget.h"

QMessageWidget::QMessageWidget(QWidget* parent) : QWidget(parent) 
{
    initializationUi();
	setLayout(mainLayout);
}

void QMessageWidget::initializationUi()
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