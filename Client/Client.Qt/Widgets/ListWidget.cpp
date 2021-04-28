#include "ListWidget.hpp"

#include "Style/Style.hpp"

ListWidget::ListWidget(QWidget* parent) : QListWidget(parent)
{
    setFont(QFont("Noto Sans", Style::valueDPIScale(12), 50));
    setStyleSheet(
        "QListWidget {"
        "background-color: #424140;"
        "color: white;"
        "}");
}
