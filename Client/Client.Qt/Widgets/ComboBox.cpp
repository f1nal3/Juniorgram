#include "ComboBox.hpp"

#include "Style/Style.hpp"

ComboBox::ComboBox(QWidget* parent) : QComboBox(parent)
{
    setFont(QFont("Noto Sans", Style::valueDPIScale(12), 55));
    setStyleSheet(
        QString("QComboBox { "
                "border: 0px;"
                "background-color: rgba(0,0,0,0);"
                "color: white;"
                "}"));
}
