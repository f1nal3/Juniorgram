#pragma once

#include <QLineEdit>
#include <QTextEdit>

#include "Style/StyleBasic.hpp"

class FlatInput : public QLineEdit
{
    Q_OBJECT
public:
    explicit FlatInput(QWidget* parent = nullptr, const QString& placeholder = QString(), bool password = false);
    [[nodiscard]] QRect getTextRect() const;

private:
    void paintEvent(QPaintEvent* event) override;
};

class FlatTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit FlatTextEdit(QWidget* parent = nullptr, const Style::FlatTextEdit& st = st::defaultTextEdit);

protected:
    void paintEvent(QPaintEvent* paintEvent) override;
    void resizeEvent(QResizeEvent* resizeEvent) override;

private:
    const Style::FlatTextEdit& _st;
};
