#include "Label.hpp"

Label::Label(const QString& text, QWidget* parent) : QLabel(text, parent)
{
    setStyleSheet(
        QString("QLabel {"
                "background-color: rgba(0,0,0,0);"
                "color: white;"
                "}"));
}
