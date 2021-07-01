#include "FlatButton.hpp"

#include <QPainter>

FlatButton::FlatButton(QWidget* parent, const QString& text, const Style::FlatButton& st)
    : AbstractButton(parent), _text(text), _st(st)
{
    setAttribute(Qt::WA_AcceptTouchEvents);

    setContentsMargins(_st.paddings);
    setMinimumHeight(_st.font->height + _st.paddings.bottom() + _st.paddings.top());
    resize(_st.font->width(_text) + _st.paddings.left() * 2, minimumHeight());
    setMinimumWidth(width());
}

void FlatButton::setText(const QString& text)
{
    _text = text;
    resize(_st.font->width(_text) + _st.paddings.left() * 2, minimumHeight());
    setMinimumWidth(width());
    update();
}

void FlatButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    auto       back  = (isOver() ? _st.overBgColor : _st.bgColor);
    const auto inner = QRect(0, 0, width(), height());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    painter.setBrush(back);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(inner, _st.rounding, _st.rounding);

    painter.setFont(isOver() ? _st.overFont : _st.font);
    painter.setPen(isOver() ? _st.overColor : _st.color);
    painter.drawText(inner.marginsRemoved(_st.paddings), _text, Style::al_top);
}

LinkButton::LinkButton(QWidget* parent, const QString& text, const Style::LinkButton& st)
    : AbstractButton(parent), _text(text), _st(st)
{
    setAttribute(Qt::WA_AcceptTouchEvents);

    setContentsMargins(0, 0, 0, 0);
    setMinimumHeight(_st.font->height);
    resize(_st.font->width(_text), minimumHeight());
    setMinimumWidth(width());
    setPointerCursor(true);
}


void LinkButton::setText(const QString& text)
{
    _text = text;
    resize(_st.font->width(_text), minimumHeight());
    setMinimumWidth(width());
    update();
}

void LinkButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    const auto inner = QRect(0, 0, width(), height());

    QPainter painter(this);
    painter.setRenderHint(QPainter::TextAntialiasing);

    painter.setFont(isOver() ? _st.overFont : _st.font);
    painter.setPen(isOver() ? _st.overColor : _st.color);
    painter.drawText(inner, _text, Style::al_top);
}
