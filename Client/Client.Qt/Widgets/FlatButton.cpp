#include "FlatButton.hpp"

FlatButton::FlatButton(QWidget* parent, const QString& text, const Style::FlatButton& st)
    : AbstractButton(parent), _text(text), _st(st)
{
    setAttribute(Qt::WA_AcceptTouchEvents);

    setContentsMargins(_st.paddings);
    setMinimumHeight(_st.font->height + _st.paddings.bottom() + _st.paddings.top());
    resize(_st.font->width(_text) + _st.paddings.left() * 2, minimumHeight());
    setMinimumWidth(width());
    setPointerCursor(true);
}
void FlatButton::setText(const QString& text) { _text = text; }

void FlatButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    auto       back  = (isOver() ? _st.overBgColor : _st.bgColor);
    const auto inner = QRect(0, 0, width(), height());

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);

    p.setBrush(back);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(inner, _st.rounding, _st.rounding);

    p.setFont(isOver() ? _st.overFont : _st.font);
    p.setPen(isOver() ? _st.overColor : _st.color);
    p.drawText(inner.marginsRemoved(_st.paddings), _text, Style::al_top);
}
