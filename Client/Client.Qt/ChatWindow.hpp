#pragma once
#include "Widgets/MessageWidget.hpp"
#include "Widgets/InputFields.hpp"
#include "Widgets/TextEdit.hpp"
#include "Widgets/ChannelListWidget.hpp"

class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    QHBoxLayout* hBoxLayout;
    QVBoxLayout* mainLayout;
    QHBoxLayout* messageEditLayout;
    QVBoxLayout* messageButtonLayout;
    explicit ChatWindow(QWidget* parent = nullptr);
    ~ChatWindow() override;

protected:
    void keyPressEvent(QKeyEvent* event) override;
private slots:
    void updateMessagesList_User();
    void updateMessagesList_Bot();
    void deletingSelection(QListWidgetItem* item);

private:
    ChannelListWidget* channelListWidget;
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
