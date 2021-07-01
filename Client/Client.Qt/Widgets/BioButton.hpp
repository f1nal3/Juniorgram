#pragma once

#include <QPropertyAnimation>
#include <QWidget>

#include "Style/Style.hpp"

/**
 * @class BioButton
 * @brief Class for Profile menu widget
 */
class BioButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor hoverColor READ getHoverColor WRITE setHoverColor)
public:
    /**
     * @brief Constructor of bio button
     * @param bioImage Profile image
     * @param inCaption Must be set true for now.
     * @param parent Parent of widget
     */
    explicit BioButton(QImage bioImage, bool inCaption = true, QWidget* parent = nullptr);

    /**
     * @brief Changes profile image (locally)
     * @param newImage
     */
    void setImage(QImage newImage);

protected:
    bool eventFilter(QObject* object, QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    /**
     * @brief Changes button background color if in title bar
     * @param newColor New background color
     */
    void setHoverColor(QColor newColor)
    {
        hoverColor = std::move(newColor);
        update();
    }
    /**
     * @brief Returns current background color
     * @return Current background color
     */
    QColor getHoverColor() { return hoverColor; }

private:
    QPropertyAnimation* fadeinAnim;
    QColor              hoverColor;
    QImage              image;
};
