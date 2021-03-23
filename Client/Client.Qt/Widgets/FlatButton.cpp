#include "FlatButton.h"

#include "Style/Style.h"

FlatButton::FlatButton(const QString& text, QWidget* parent) : QPushButton(text, parent)
{
    setFont(QFont("JetBrains Mono", Style::valueDPIScale(12)));
    setStyleSheet(
        QString("QPushButton { "
                "border: 0px;"
                "background-color: rgba(0,0,0,0);"
                "color: white;"
                "padding:9px;"
                "}"));
    setAttribute(Qt::WA_AcceptTouchEvents);

    this->setMouseTracking(true);
}
void FlatButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QColor inputField(0x32, 0x32, 0x32);
    inputField = inputField.lighter(175);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(inputField);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(QRectF(0, 0, width(), height()).marginsRemoved(QMarginsF(2, 1, 2, 1)),
                      Style::valueDPIScale(5), Style::valueDPIScale(5));
    QPushButton::paintEvent(event);
}
