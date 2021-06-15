#include "ComboBox.hpp"

#include "Style/Style.hpp"
#include "Style/StyleBasic.hpp"

ComboBox::ComboBox(QWidget* parent) : QComboBox(parent)
{
    setFont(st::semiboldFont);
    setStyleSheet(
        QString("QComboBox { "
                "border: 0px;"
                "background-color: rgba(0,0,0,255);"
                "color: white;"
                "}"));
    setContentsMargins(st::mar);
}
