#pragma once
#include "MessageWidget.hpp"
#include "Widgets/InputFields.hpp"
#include "Widgets/TextEdit.hpp"
#include "pch.hpp"

class ChatWindow : public QWidget
{
Q_OBJECT
public:
    QVBoxLayout* mainLayout;
    QHBoxLayout* messageEditLayout;
    QVBoxLayout* messageButtonLayout;
    explicit ChatWindow(QWidget* parent = nullptr);
    ~ChatWindow() override;


protected:
    void keyPressEvent(QKeyEvent* event) override;
public slots:
    void updateMessagesList_User();
    void updateMessagesList_Bot();

private:
    FlatButton* sendButton;
    FlatButton* botButton;
    TextEdit* textEdit;
    QListWidget* chatWidget;
    QSpacerItem* verticalUpSpacer;
    QSpacerItem* verticalDownSpacer;
    void connectButton();
    void newMessage(QString textMessage, QString userNameMessage);
    void newMessage(QString textMessage);

};
