#include "FlatButton.hpp"

#include "Style/Style.hpp"

FlatButton::FlatButton(const QString& text, QWidget* parent) : QPushButton(text, parent), hit(false)
{
    auto font = QFont("Noto Sans", 12);
    font.setPixelSize(Style::valueDPIScale(15));
    setFont(font);

    setStyleSheet(
        QString("QPushButton { "
                "border: 0px;"
                "background-color: rgba(0,0,0,0);"
                "color: white;"
                "}"));
    setAttribute(Qt::WA_AcceptTouchEvents);

    this->setMouseTracking(true);
    inputField = inputField.lighter(175);
    setContentsMargins(Style::valueDPIScale(8), Style::valueDPIScale(8), Style::valueDPIScale(8),
                       Style::valueDPIScale(8));
    setMinimumHeight(fontMetrics().height() + Style::valueDPIScale(8) * 2);
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
    p.drawRoundedRect(QRectF(0, 0, width(), height()), Style::valueDPIScale(5),
                      Style::valueDPIScale(5));
    QPushButton::paintEvent(event);
}
QSize FlatButton::sizeHint() const
{
    const auto parentHint = QPushButton::sizeHint();
    return QSize(parentHint.width() + Style::valueDPIScale(8) * 2,
                 parentHint.height() + Style::valueDPIScale(8) * 2);
}
void FlatButton::leaveEvent(QEvent* e)
{
    Q_UNUSED(e)
    hit = false;
}
void FlatButton::enterEvent(QEvent* e)
{
    Q_UNUSED(e)
    hit = true;
}
