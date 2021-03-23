#pragma once

#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QString>
#include <QWidget>
#include <cstdlib>
#include <iostream>
class TextEdit : public QWidget
{
    Q_OBJECT
public:
    TextEdit(QWidget* parent = nullptr);
    QString text() const;
    void clear();
    //~TextEdit();

private:
    QPushButton* boldButton;
    QPlainTextEdit* textField;

private slots:
    void boldButtonClicked();
};