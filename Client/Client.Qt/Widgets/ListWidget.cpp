#include "ListWidget.hpp"

#include "Style/Style.hpp"

ListWidget::ListWidget(QWidget* parent) : QListWidget(parent)
{
    setStyleSheet(
        "QListWidget {"
        "background-color: #424140;"
        "color: white;"
        "}");
}
