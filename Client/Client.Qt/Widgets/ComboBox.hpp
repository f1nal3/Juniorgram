#pragma once

#include <QComboBox>

/**
 * @class ComboBox
 * @brief Custom style for ComboBox widget
 */
class ComboBox : public QComboBox
{
public:
    /**
     * @brief Constructor for ComboBox
     * @param parent parent widget
     */
    explicit ComboBox(QWidget* parent = nullptr);
};
