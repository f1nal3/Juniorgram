#include "ListWidget.hpp"

ListWidget::ListWidget(QWidget* parent) : QListWidget(parent)
{
    setStyleSheet(
        "QListWidget {"
        "background-color: #424140;"
        "color: white;"
        "border: none;"
        "}");

    _switchItemUp = std::make_unique<QShortcut>
    (
        QKeySequence{ "Alt+P" }, 
        this, 
        [this]()
        {
            setCurrentRow(currentRow() <= 0 ? count() - 1 : currentRow() - 1);
            if (currentItem())
            {
                emit itemPressed(currentItem());
            }
        }
    );

    _switchItemDown = std::make_unique<QShortcut>
    (
        QKeySequence{ "Alt+N" },
        this,
        [this]()
        {
            setCurrentRow(currentRow() == count() - 1 ? 0 : currentRow() + 1);
            if (currentItem())
            {
                emit itemPressed(currentItem());
            }
        }
    );
}
