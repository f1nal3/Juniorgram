#include "FlatButton.h"

#include "Style/Style.h"

FlatButton::FlatButton(const QString& text, QWidget* parent) : QPushButton(text, parent)
{
    setFont(QFont("Noto Sans", Style::valueDPIScale(12)));
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
    p.setBrush(inputField);
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