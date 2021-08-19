#pragma once

#include <QListWidget>

/**
 *  @class ListWidget
 *  @brief Custom style for QListWidget
 */
class ListWidget : public QListWidget
{
public:
    /// Constructor for List Widget
    explicit ListWidget(QWidget* parent = nullptr);
};
