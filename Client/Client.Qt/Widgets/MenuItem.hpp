#pragma once
#include "pch.hpp"

class MenuItem : public QWidget
{
    Q_OBJECT
public:
    explicit MenuItem(const QString& text, QAction* action, QWidget* parent = nullptr);

protected:
    [[nodiscard]] QSize sizeHint() const override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    bool hit{};
    QString innerText;
    QAction* mAction;
};
