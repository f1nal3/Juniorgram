#include "DateTimeEdit.hpp"

#include "Style/Style.hpp"

DateTimeEdit::DateTimeEdit(QWidget* parent) : QDateTimeEdit(parent)
{
    setFont(QFont("Noto Sans", Style::valueDPIScale(12), 50));
    setStyleSheet(
                QString("QDateTimeEdit { "
                        "border: 0px;"
                        "background-color: rgba(0,0,0,0);"
                        "color: white;"
                        "}"));
    setFrame(false);
    setReadOnly(true);
    setButtonSymbols(QAbstractSpinBox::NoButtons);
    setKeyboardTracking(true);
}
