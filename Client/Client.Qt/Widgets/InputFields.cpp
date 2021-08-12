#include "InputFields.hpp"

#include <QCommonStyle>
#include <QCoreApplication>
#include <QDebug>
#include <QPainter>
#include <QRegularExpressionValidator>
#include <QtEvents>

#include "Style/StyleBasic.hpp"

/// This is to remove default qt drawing
template <typename InputClass>
class InputStyle : public QCommonStyle
{
public:
    InputStyle() { setParent(QCoreApplication::instance()); }

    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
                       const QWidget* widget = nullptr) const override
    {
        Q_UNUSED(element)
        Q_UNUSED(option)
        Q_UNUSED(painter)
        Q_UNUSED(widget)
    }
    QRect subElementRect(SubElement r, const QStyleOption* opt, const QWidget* widget = nullptr) const override
    {
        switch (r)
        {
            case SE_LineEditContents:
            {
                const auto w = widget ? qobject_cast<const InputClass*>(widget) : nullptr;
                return w ? w->getTextRect() : QCommonStyle::subElementRect(r, opt, widget);
                break;
            }
            default:
                break;
        }
        return QCommonStyle::subElementRect(r, opt, widget);
    }

    static InputStyle<InputClass>* instance()
    {
        if (!_instance)
        {
            if (!QCoreApplication::instance())
            {
                return nullptr;
            }
            _instance = new InputStyle<InputClass>();
        }
        return _instance;
    }

    ~InputStyle() override { _instance = nullptr; }

private:
    static InputStyle<InputClass>* _instance;
};

template <typename InputClass>
InputStyle<InputClass>* InputStyle<InputClass>::_instance = nullptr;

FlatInput::FlatInput(QWidget* parent, const QString& placeholder, bool password) : QLineEdit(parent)
{
    setFont(st::defaultFont);
    auto* regexpvalidator = new QRegularExpressionValidator;
    regexpvalidator->setRegularExpression(QRegularExpression("[a-zA-Z0-9._]+@[a-zA-Z0-9]+.[a-zA-Z]+"));
    setValidator(regexpvalidator);
    setStyle(InputStyle<FlatInput>::instance());
    auto p = palette();
    p.setColor(QPalette::Window, Qt::white);
    p.setColor(QPalette::Text, Qt::white);
    p.setColor(QPalette::Highlight, Qt::gray);
    p.setColor(QPalette::HighlightedText, Qt::white);
    setPalette(p);

    setAttribute(Qt::WA_AcceptTouchEvents);

    QLineEdit::setTextMargins(0, 0, 0, 0);
    setContentsMargins(st::mar);
    setMinimumHeight(fontMetrics().height() + Style::valueDPIScale(8) * 2);
    setPlaceholderText(placeholder);
    setEchoMode(password ? Password : Normal);
}

QRect FlatInput::getTextRect() const { return rect().marginsRemoved(st::mar + QMargins(-2, -1, -2, -1)); }

void FlatInput::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QColor inputField(0x32, 0x32, 0x32);
    inputField = inputField.darker(175);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(inputField);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(QRectF(0, 0, width(), height()), Style::valueDPIScale(8), Style::valueDPIScale(8));

    QLineEdit::paintEvent(event);
}

FlatTextEdit::FlatTextEdit(QWidget* parent, const Style::FlatTextEdit& st) : QTextEdit(parent), _st(st)
{
    setFont(_st.font);
    setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    viewport()->setAutoFillBackground(false);

    auto p = palette();
    p.setColor(QPalette::Text, _st.textcolor->color);
    p.setColor(QPalette::Highlight, _st.selectedcolor->color);
    p.setColor(QPalette::HighlightedText, _st.selectedtextcolor->color);
    setPalette(p);

    setContentsMargins(_st.margins);
    document()->setDocumentMargin(_st.margins.left());
    setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setAttribute(Qt::WA_AcceptTouchEvents);
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents);

    setMinimumHeight(_st.margins.top() + _st.margins.bottom() + _st.font->height);
}

void FlatTextEdit::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter painter(this->viewport());

    painter.setPen(Qt::NoPen);

    painter.setBrush(_st.bg);
    painter.drawRect(rect());

    QTextEdit::paintEvent(event);
}

void FlatTextEdit::resizeEvent(QResizeEvent* resizeEvent) { QTextEdit::resizeEvent(resizeEvent); }
