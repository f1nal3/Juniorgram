#pragma once

#include <QLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QString>
#include <QWidget>
class TextEdit : public QWidget
{
    Q_OBJECT
public:
    TextEdit(QWidget* parent = nullptr);
    QString text() const;
    void clear();
    ~TextEdit();

private:
    QPushButton* boldButton;
    QPlainTextEdit* textField;

private slots:
    void boldButtonClicked();
};
