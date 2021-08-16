#include "Label.hpp"

#include "Style/Styles.hpp"

Label::Label(const QString& text, QWidget* parent) : QLabel(text, parent)
{
    setFont(st::defaultFont);
    setAttribute(Qt::WA_TranslucentBackground);
    auto p = palette();
    p.setColor(QPalette::Text, Qt::white);
    p.setColor(QPalette::BrightText, Qt::white);
    p.setColor(QPalette::WindowText, Qt::white);
    setPalette(p);
}
