#pragma once
#include "pch.h"
#include "Widgets/TextEdit.hpp"
#include "Widgets/InputFields.hpp"
#include "MessageWidget.hpp"

class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWindow(QWidget* parent = nullptr);
    ~ChatWindow() override;

private:
    FlatButton* sendButton;
    FlatButton* randomButton;
    TextEdit* textEdit;
    QListWidget* chatWidget;
    void connectButton();
    void newMessage(QString textMessage, QString userNameMessage);
    void newMessage(QString textMessage);

public slots:
    void updateMessagesList_User();
    void updateMessagesList_Bot();

};
