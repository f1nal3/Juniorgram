#pragma once
#include <Style/Style.hpp>

#include "pch.hpp"

class BioButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor hoverColor READ getHoverColor WRITE setHoverColor)
public:
    /**
     * Constructor of bio button
     * @param bioImage Profile image
     * @param inCaption Must be set true for now.
     * @param parent Parent of widget
     */
    explicit BioButton(QImage bioImage, bool inCaption = true, QWidget* parent = nullptr);

    /**
     * Changes profile image (locally)
     * @param newImage
     */
    void setImage(QImage newImage);

protected:
    bool eventFilter(QObject* object, QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    /**
     * Changes button background color if in title bar
     * @param newColor New background color
     */
    void setHoverColor(QColor newColor)
    {
        hoverColor = std::move(newColor);
        repaint();
    }
    /**
     * Returns current background color
     * @return Current background color
     */
    QColor getHoverColor() { return hoverColor; }

private:
    QPropertyAnimation* fadeinAnim;
    QColor hoverColor;
    QImage image;
};
