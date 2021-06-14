#include "DateTimeEdit.hpp"

#include "Style/Style.hpp"

DateTimeEdit::DateTimeEdit(QWidget* parent) : QDateTimeEdit(parent)
{
    setStyleSheet(
        QString("QDateTimeEdit { "
                "border: 0px;"
                "background-color: rgba(0,0,0,0);"
                "color: white;"
                "}"));
    setFrame(false);
    setReadOnly(true);
    setButtonSymbols(QAbstractSpinBox::NoButtons);
}
