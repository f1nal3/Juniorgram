#include "ListWidget.hpp"

ListWidget::ListWidget(QWidget* parent) : QListWidget(parent)
{
    setStyleSheet(
        "QListWidget {"
        "background-color: #424140;"
        "color: white;"
        "border: none;"
        "}");
}
