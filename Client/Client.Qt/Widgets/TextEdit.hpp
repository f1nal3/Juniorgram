#pragma once

#include "FlatButton.hpp"
#include "InputFields.hpp"
#include "pch.hpp"
class TextEdit : public QWidget
{
    Q_OBJECT
public:
    explicit TextEdit(QWidget* parent = nullptr);
    [[nodiscard]] QString text() const;
    void clear();
    ~TextEdit() override;

private:
    FlatButton* mBoldButton;
    FlatPlainTextEdit* mTextField;
    
    const int boldSymbolSize = 2;

private slots:
    void boldButtonClicked();
};
