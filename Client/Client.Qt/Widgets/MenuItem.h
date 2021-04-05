#pragma once
#include "pch.h"

class MenuItem : public QWidget
{
    Q_OBJECT
public:
    explicit MenuItem(const QString& text, QAction* action, QWidget* parent = nullptr);

protected:
    [[nodiscard]] QSize sizeHint() const override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void leaveEvent(QEvent* e) override;
    void paintEvent(QPaintEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

private:
    bool hit{};
    QString _text;
    QAction* _action;
};
