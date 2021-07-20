#pragma once

#include "Style/StyleBasic.hpp"
#include "Widgets/CaptionButton.hpp"

/**
 * @class BioButton
 * @brief Class for Profile menu widget
 */
class BioButton : public CaptionButton
{
    Q_OBJECT
public:
    /**
     * @brief Constructor of bio button
     * @param parent Parent of widget
     * @param inCaption Must be set true for now
     */
    explicit BioButton(QWidget* parent = nullptr, bool inCaption = true);
};
