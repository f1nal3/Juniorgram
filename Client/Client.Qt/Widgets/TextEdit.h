#pragma once

#include "FlatButton.h"
#include "InputFields.h"
#include "pch.h"
class TextEdit : public QWidget
{
    Q_OBJECT
public:
    explicit TextEdit(QWidget* parent = nullptr);
    [[nodiscard]] QString text() const;
    void clear();
    ~TextEdit() override;

private:
    FlatButton* boldButton;
    FlatPlainTextEdit* textField;

private slots:
    void boldButtonClicked();
};
