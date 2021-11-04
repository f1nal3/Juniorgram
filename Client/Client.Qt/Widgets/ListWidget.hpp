#pragma once

#include <QListWidget>
#include <QShortcut>

/**
 *  @class ListWidget
 *  @brief Custom style for QListWidget
 */
class ListWidget : public QListWidget
{
    Q_OBJECT

    std::unique_ptr<QShortcut>      _switchItemUp;
    std::unique_ptr<QShortcut>      _switchItemDown;
public:
    /// Constructor for List Widget
    explicit ListWidget(QWidget* parent = nullptr);
};
