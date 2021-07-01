#pragma once
#include "Widgets/InputFields.hpp"
#include "Widgets/TextEdit.hpp"
#include "Widgets/MessageWidget.hpp"

class ChatWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ChatWidget(QWidget* parent = nullptr);
    ~ChatWidget() override;

private slots:
    void deletingSelection(QListWidgetItem* item);
    void newMessage(QString messageText);
    void newMessage(QString textMessage, QString userNameMessage);

private:
    QVBoxLayout* mainChatLayout;
    QListWidget* chatWidget;
    TextEdit* textEdit;
    FlatButton* sendButton;
    FlatPlainTextEdit* messageTextEdit;
    void connectUi();
};
