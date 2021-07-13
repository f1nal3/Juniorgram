#pragma once
#include "Widgets/InputFields.hpp"
#include "Widgets/MessageWidget.hpp"
#include "Widgets/TextEdit.hpp"

class ChatHistory;

class ChatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWidget(QWidget* parent = nullptr);
    ~ChatWidget() override;

private slots:
    void newMessage(const QString& messageText);
    void newMessage(const QString& messageText, const QString& userNameMessage);

private:
    void connectUi();

private:
    QVBoxLayout* mainChatLayout;
    ChatHistory* chatWidget;
    TextEdit*    textEdit;
};
