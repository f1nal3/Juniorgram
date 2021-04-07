#pragma once

#include "pch.hpp"

class FlatButton : public QPushButton
{
public:
    explicit FlatButton(const QString& text, QWidget* parent = nullptr);

protected:
    [[nodiscard]] QSize sizeHint() const override;
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEvent* e) override;
    void leaveEvent(QEvent* e) override;

private:
    QColor inputField{0x32, 0x32, 0x32};
    QColor lastOne;
    bool hit;
};
