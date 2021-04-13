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
    const QString boldSymbol = "**";

    void delSymbolsInSelection(QString &text, int &start, int &end, int symbolSize);
    void delSymbolsOutSelection(QString &text, int &start, int &end, int symbolSize);
    void insertSymbolsInSelection(QTextCursor &cursor, int &start, int &end, int symbolSize, const QString symbol);
    void selectText(QTextCursor &cursor, int start, int end);

private slots:
    void boldButtonClicked();
};
