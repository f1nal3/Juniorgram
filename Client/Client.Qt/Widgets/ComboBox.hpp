#pragma once

#include <QComboBox>

/**
 * @class ComboBox
 * @brief Custom style for ComboBox widget
 */
class ComboBox : public QComboBox
{
public:
    /// Constructor for ComboBox
    explicit ComboBox(QWidget* parent = nullptr);
};
