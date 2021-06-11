#include "Label.hpp"

#include "Style/StyleBasic.hpp"

Label::Label(const QString& text, QWidget* parent) : QLabel(text, parent)
{
    setFont(st::defaultFont);
    setAttribute(Qt::WA_TranslucentBackground);
    auto p = palette();
    p.setColor(QPalette::Text, Qt::white);
    setPalette(p);
}
