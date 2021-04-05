#pragma once
#include "Widgets/TextEdit.h"
#include "Widgets/InputFields.h"
#include "pch.h"
#include "MessageWidget.hpp"

class ChatWindow : public QWidget
{
Q_OBJECT
public:
    QVBoxLayout* mainLayout;
    QHBoxLayout* messageEditLayout;
    QVBoxLayout* messageButtonLayout;
    explicit ChatWindow(QWidget* parent = nullptr);
    void keyPressEvent(QKeyEvent* event);
    ~ChatWindow();

public slots:
    void updateMessagesList_User();
    void updateMessagesList_Bot();

private:
    QPushButton* sendButton;
    QPushButton* botButton;
    TextEdit* textEdit;
    QListWidget* chatWidget;
    QSpacerItem* verticalUpSpacer;
    QSpacerItem* verticalDownSpacer;
    void connectButton();
    void newMessage(QString textMessage, QString userNameMessage);
    void newMessage(QString textMessage);
};
