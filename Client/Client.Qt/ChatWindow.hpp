#pragma once
#include "Widgets/MessageWidget.hpp"
#include "Widgets/InputFields.hpp"
#include "Widgets/TextEdit.hpp"
#include "Widgets/ChannelListWidget.hpp"
#include "Widgets/ChatWidget.hpp"

class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWindow(QWidget* parent = nullptr);
    ~ChatWindow() override;

protected:
    void keyPressEvent(QKeyEvent* event) override;
private slots:
    void updateMessagesList_User();
    void deletingSelection(QListWidgetItem* item);

private:
    QHBoxLayout* mainLayout;
    QVBoxLayout* rightLayout;
    ChannelListWidget* channelListWidget;
    ChatWidget* chat;
    //del
    FlatButton* sendButton;
    FlatPlainTextEdit* messageTextEdit;
    TextEdit* textEdit;
    QListWidget* chatWidget;
    void connectUi();
    void newMessage(QString textMessage, QString userNameMessage);
    void newMessage(QString textMessage);
};
