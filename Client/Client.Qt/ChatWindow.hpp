#pragma once
#include "Widgets/InputFields.hpp"
#include "Widgets/TextEdit.hpp"
#include "pch.h"

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
};
