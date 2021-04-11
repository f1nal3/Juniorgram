#pragma once
#include <Style/Style.hpp>

#include "pch.hpp"

class BioButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor hoverColor READ getHoverColor WRITE setHoverColor)
public:
    explicit BioButton(QImage bioImage, bool inCaption = true, QWidget* parent = nullptr);

    void setImage(QImage newImage);

protected:
    bool eventFilter(QObject* object, QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    void setHoverColor(QColor newColor)
    {
        hoverColor = std::move(newColor);
        repaint();
    }
    QColor getHoverColor() { return hoverColor; }

private:
    QPropertyAnimation* fadeinAnim;
    QColor hoverColor;
    QImage image;
};
