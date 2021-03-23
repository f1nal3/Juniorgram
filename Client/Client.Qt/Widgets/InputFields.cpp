#include "InputFields.h"

FlatInput::FlatInput(QWidget* parent) : QLineEdit(parent)
{
    setFont(QFont("JetBrains Mono", Style::valueDPIScale(12)));
    QColor inputField(0x32, 0x32, 0x32);
    inputField            = inputField.lighter(175);
    auto selectedText     = inputField.lighter(175);
    auto* regexpvalidator = new QRegExpValidator;
    regexpvalidator->setRegExp(QRegExp("[a-zA-Z0-9._]+@[a-zA-Z0-9]+.[a-zA-Z]+"));
    setValidator(regexpvalidator);
    // TODO: This part is stupid, implement it paintEvent
    setStyleSheet(QString("QLineEdit { "
                          "border: 0px;"
                          "selection-background-color: rgb(%1, %2, %3);"
                          "background-color: rgba(0,0,0,0);"
                          "color:white;"
                          "}")
                      .arg(selectedText.red())
                      .arg(selectedText.green())
                      .arg(selectedText.blue()));

    setAttribute(Qt::WA_AcceptTouchEvents);

    QLineEdit::setTextMargins(0, 0, 0, 0);
    setContentsMargins(9, 9, 9, 9);
}
void FlatInput::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QColor inputField(0x32, 0x32, 0x32);
    inputField = inputField.lighter(175);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(inputField);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(QRectF(0, 0, width(), height()).marginsRemoved(QMarginsF(2, 2, 2, 2)),
                      Style::valueDPIScale(5), Style::valueDPIScale(5));

    QLineEdit::paintEvent(event);
}
FlatInput::FlatInput(const QString& placeholder, bool password, QWidget* parent) : FlatInput(parent)
{
    setPlaceholderText(placeholder);
    setEchoMode(password ? Password : Normal);
}
