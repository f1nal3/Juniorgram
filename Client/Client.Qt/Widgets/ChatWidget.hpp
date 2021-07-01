#pragma once
#include "Widgets/InputFields.hpp"
#include "Widgets/TextEdit.hpp"

class ChatWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ChatWidget(QWidget* parent = nullptr);
    ~ChatWidget() override;

private slots:
    void deletingSelection(QListWidgetItem* item);

private:
    QVBoxLayout* mainChatLayout;
    QListWidget* chatWidget;
    TextEdit* textEdit;
    FlatButton* sendButton;
    FlatPlainTextEdit* messageTextEdit;
    void connectUi();
};
