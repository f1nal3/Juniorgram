#pragma once
#include "MessageWidget.hpp"
#include "Widgets/InputFields.hpp"
#include "Widgets/TextEdit.hpp"
#include "Widgets/ChannelListWidget.hpp"

class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    QHBoxLayout* hBox;
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
    std::unique_ptr<ChannelListWidget> channelListWidget;
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
