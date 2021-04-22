#include "Label.hpp"

#include "Style/Style.hpp"

Label::Label(const QString &text, QWidget *parent)
    : QLabel(parent)
{
    setFont(QFont("NotoSans", Style::valueDPIScale(12), 50));
    setStyleSheet(
                QString("QLabel { "
                        "border: 0px;"
                        "background-color: rgba(0,0,0,0);"
                        "color: white;"
                        "}"));
    setText(text);
}
