#pragma once
#include "pch.hpp"

class LogoWidget : public QWidget
{
    Q_OBJECT
public:
    LogoWidget(QWidget* parent = nullptr);
    ~LogoWidget();

    QSize bestFit() const { return logo->size(); }

protected:
    void paintEvent(QPaintEvent* paintEvent);

private:
    QPixmap* logo;
};
