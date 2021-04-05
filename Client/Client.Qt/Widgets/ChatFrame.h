#pragma once
#include "pch.h"
#include "Widgets/TextEdit.h"

class ChatFrame : public QWidget
{
    Q_OBJECT
public:
    explicit ChatFrame(QWidget* parent=nullptr);
    ~ChatFrame() override;

private:
    TextEdit* textEdit;
};
