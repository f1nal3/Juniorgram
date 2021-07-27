#pragma once

#include <QComboBox>

/** @class ComboBox
 *  @brief Custom style for ComboBox object
 */
class ComboBox: public QComboBox
{
public:
    /**
    *  @brief Setting the style for ComboBox objects
    */
    explicit ComboBox(QWidget* parent = nullptr);
};
