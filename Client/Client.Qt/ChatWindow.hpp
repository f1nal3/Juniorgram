#pragma once
#include "Widgets/TextEdit.h"
#include "Widgets/InputFields.h"
#include "pch.h"
#include "MessageWidget.hpp"

class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWindow(QWidget* parent = nullptr);
    ~ChatWindow();

public slots:
    void updateMessagesList_User();
    void updateMessagesList_Bot();

private:
    QPushButton* sendButton;
    QPushButton* randomButton;
    TextEdit* textEdit; 
    QListWidget* chatWidget;
    void connectButton();
    void newMessage(QString textMessage, QString userNameMessage);
    void newMessage(QString textMessage);
};
