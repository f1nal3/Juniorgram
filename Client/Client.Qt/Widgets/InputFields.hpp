#pragma once

#include "Style/Style.hpp"
#include "pch.hpp"

class FlatInput : public QLineEdit
{
    Q_OBJECT
public:
    explicit FlatInput(QWidget* parent = nullptr);
    explicit FlatInput(const QString& placeholder, QWidget* parent = nullptr);
    explicit FlatInput(const QString& placeholder, bool password = false,
                       QWidget* parent = nullptr);
    QRect getTextRect() const;

private:
    void paintEvent(QPaintEvent* event) override;
};

class FlatPlainTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit FlatPlainTextEdit(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
};
