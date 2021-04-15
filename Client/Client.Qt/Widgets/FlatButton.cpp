#include "FlatButton.hpp"

#include "Style/Style.hpp"

FlatButton::FlatButton(const QString& text, QWidget* parent) : QPushButton(text, parent),hit(false)
{
    setFont(QFont("Noto Sans", Style::valueDPIScale(12),50));
    setStyleSheet(
        QString("QPushButton { "
                "border: 0px;"
                "background-color: rgba(0,0,0,0);"
                "color: white;"
                "}"));
    setAttribute(Qt::WA_AcceptTouchEvents);

    this->setMouseTracking(true);
    inputField = inputField.lighter(175);
    lastOne    = inputField;
}

void FlatButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QColor back = inputField;
    if (hit) back = back.lighter(125);
    p.setBrush(back);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(QRectF(0, 0, width(), height()).marginsRemoved(QMarginsF(2, 1, 2, 1)),
                      Style::valueDPIScale(5), Style::valueDPIScale(5));
    QPushButton::paintEvent(event);
}
QSize FlatButton::sizeHint() const
{
    const auto parentHint = QPushButton::sizeHint();
    return QSize(parentHint.width() + 18, parentHint.height() + 18);
}
void FlatButton::leaveEvent(QEvent* e)
{
    Q_UNUSED(e);
    hit = false;
}
void FlatButton::enterEvent(QEvent* e)
{
    Q_UNUSED(e);
    hit = true;
}
