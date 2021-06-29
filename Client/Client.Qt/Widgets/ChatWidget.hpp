#pragma once
#include "Widgets/MessageWidget.hpp"
#include "Widgets/InputFields.hpp"
#include "Widgets/TextEdit.hpp"

class ChatWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ChatWidget(QWidget* parent = nullptr);
    ~ChatWidget() override;

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void updateMessagesList_User();
    void deletingSelection(QListWidgetItem* item);

private:
    QVBoxLayout* mainChatLayout;
    QListWidget* chatWidget;
    TextEdit* textEdit;
    FlatButton* sendButton;
    FlatPlainTextEdit* messageTextEdit;
    void connectUi();
    void newMessage(QString textMessage, QString userNameMessage);
    void newMessage(QString textMessage);
};
