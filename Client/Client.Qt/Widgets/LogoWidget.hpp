#pragma once
#include "pch.hpp"
#include <QPainter>

class LogoWidget : public QWidget
{
    Q_OBJECT
public:
    LogoWidget(QWidget* parent = nullptr);
    ~LogoWidget();

    QSize bestFit() const { return logo->size(); }

protected:
    void paintEvent(QPaintEvent* event);

private:
    QPixmap* logo;
};
