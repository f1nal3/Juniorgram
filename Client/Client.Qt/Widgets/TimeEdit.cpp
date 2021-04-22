#include "TimeEdit.hpp"

#include "Style/Style.hpp"

TimeEdit::TimeEdit(QWidget* parent) : QTimeEdit(parent)
{
    setFont(QFont("NotoSans", Style::valueDPIScale(12),50));
    setStyleSheet(
                QString("QTimeEdit { "
                        "border: 0px;"
                        "background-color: rgba(0,0,0,0);"
                        "color: white;"
                        "}"));
    setFrame(false);
    setReadOnly(true);
    setButtonSymbols(QAbstractSpinBox::NoButtons);
    setKeyboardTracking(true);
}
