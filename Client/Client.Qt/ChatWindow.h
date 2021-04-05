#pragma once
#include "Widgets/TextEdit.h"
#include "Widgets/InputFields.h"
#include "pch.h"

class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWindow(QWidget* parent = nullptr);
    ~ChatWindow();

private:
    QPushButton* sendButton;
    QPushButton* randomButton;
    TextEdit* textEdit; 
    QListWidget* chatWidget;
};
