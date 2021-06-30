#pragma once

#include <QListWidget>

/** @class TimeEdit
 *  @brief Custom style for QListWidget
 */
class ListWidget : public QListWidget
{
public:
    explicit ListWidget(QWidget* parent = nullptr);
};
