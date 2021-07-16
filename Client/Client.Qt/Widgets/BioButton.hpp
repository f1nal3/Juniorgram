#pragma once

#include <QPropertyAnimation>
#include <QWidget>

#include "Style/Style.hpp"
#include "Widgets/CaptionButton.hpp"

/**
 * @class BioButton
 * @brief Class for Profile menu widget
 */
class BioButton : public AbstractButton
{
    Q_OBJECT
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
    void paintEvent(QPaintEvent* event) override;

private:
    QImage image;
};
