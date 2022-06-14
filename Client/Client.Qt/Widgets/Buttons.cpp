#include "Buttons.hpp"

#include <QDebug>
#include <QPainter>

FlatButton::FlatButton(QWidget* parent, const QString& text, const Style::FlatButton& st) : AbstractButton(parent), _text(text), _st(st)
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

    auto       back  = isOver() ? _st.overBgColor : _st.bgColor;
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

LinkButton::LinkButton(QWidget* parent, const QString& text, const Style::LinkButton& st) : AbstractButton(parent), _text(text), _st(st)
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

IconButton::IconButton(QWidget* parent, QString text, const Style::IconButton& st) : AbstractButton(parent), _text(std::move(text)), _st(st)
{
    setAttribute(Qt::WA_AcceptTouchEvents);

    updateWidget();
}

void IconButton::setText(const QString& text)
{
    _text = text;
    updateWidget();
}

void IconButton::paintEvent(QPaintEvent* event)
{
    QPainter p(this);

    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    p.setPen(Qt::NoPen);
    p.setBrush(isOver() ? _st.overBgColor : _st.bgColor);
    p.drawRoundedRect(rect(), _st.rounding, _st.rounding);
    const auto& icon = _iconOverride ? *_iconOverride : _st.icon;

    p.drawPixmap(_st.margins.left(), (height() - icon.size().height()) / 2, *icon);
    const Style::font& currentFont = isOver() ? _st.overFont : _st.font;
    p.setFont(currentFont);
    p.setPen(isOver() ? _st.overColor : _st.color);
    auto textSize  = QSize(_st.font->width(_text), height() - _st.margins.bottom());
    auto textPoint = QPoint(_st.margins.left() + _st.margins.right() + icon.size().width(), _st.margins.top());
    auto textRect  = QRect(textPoint, textSize);

    p.drawText(textRect, _text, Style::al_left);

    QWidget::paintEvent(event);
}

void IconButton::updateWidget()
{
    const int   longestFont = std::max(_st.font->width(_text), _st.overFont->width(_text));
    const auto& icon        = _iconOverride ? *_iconOverride : _st.icon;
    setMinimumWidth(_st.margins.left() + _st.margins.right() * 2 + icon.size().width() + longestFont);
    const int biggestFont = std::max(icon.size().height(), std::max(_st.font->height, _st.overFont->height));
    setMinimumHeight(_st.margins.top() + _st.margins.bottom() + biggestFont);
    resize(minimumWidth(), minimumHeight());
}

void IconButton::setIcon(const Style::icon* icon)
{
    _iconOverride = icon;
    updateWidget();
}
