#include "InputFields.hpp"

#include "Style/StyleBasic.hpp"

#include <QCommonStyle>
#include <QStyle>

template <typename InputClass>
class InputStyle : public QCommonStyle
{
public:
    InputStyle() { setParent(QCoreApplication::instance()); }

    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
                       const QWidget* widget = nullptr) const override
    {
        Q_UNUSED(element);
        Q_UNUSED(option);
        Q_UNUSED(painter);
        Q_UNUSED(widget);
    }
    QRect subElementRect(SubElement r, const QStyleOption* opt,
                         const QWidget* widget = nullptr) const override
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
            if (!QGuiApplication::instance())
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

FlatInput::FlatInput(QWidget* parent) : QLineEdit(parent)
{
    setFont(st::defaultFont);
    auto* regexpvalidator = new QRegExpValidator;
    regexpvalidator->setRegExp(QRegExp("[a-zA-Z0-9._]+@[a-zA-Z0-9]+.[a-zA-Z]+"));
    setValidator(regexpvalidator);
    setStyle(InputStyle<FlatInput>::instance());
    auto p = palette();
    p.setColor(QPalette::Text, Qt::white);
    p.setColor(QPalette::Highlight, Qt::white);
    p.setColor(QPalette::HighlightedText, Qt::white);
    setPalette(p);

    setAttribute(Qt::WA_AcceptTouchEvents);

    QLineEdit::setTextMargins(0, 0, 0, 0);
    setContentsMargins(st::mar);
    setMinimumHeight(fontMetrics().height() + Style::valueDPIScale(8) * 2);
}

void FlatInput::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QColor inputField(0x32, 0x32, 0x32);
    inputField = inputField.darker(175);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(inputField);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(QRectF(0, 0, width(), height()), Style::valueDPIScale(8),
                      Style::valueDPIScale(8));

    QLineEdit::paintEvent(event);
}

FlatInput::FlatInput(const QString& placeholder, bool password, QWidget* parent) : FlatInput(parent)
{
    setPlaceholderText(placeholder);
    setEchoMode(password ? Password : Normal);
}

FlatInput::FlatInput(const QString& placeholder, QWidget* parent)
    : FlatInput(placeholder, false, parent)
{
}
QRect FlatInput::getTextRect() const
{
    return rect().marginsRemoved(st::mar + QMargins(-2, -1, -2, -1));
}

FlatPlainTextEdit::FlatPlainTextEdit(QWidget* parent) : QTextEdit(parent)
{
    setFont(st::defaultFont);
    QColor inputField(0x32, 0x32, 0x32);
    inputField        = inputField.lighter(175);
    auto selectedText = inputField.lighter(120);

    setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    viewport()->setAutoFillBackground(false);

    auto p = palette();
    p.setColor(QPalette::Text, Qt::white);
    p.setColor(QPalette::Highlight, selectedText);
    p.setColor(QPalette::HighlightedText, inputField);
    setPalette(p);

    setContentsMargins(0, 0, 0, 0);
    document()->setDocumentMargin(st::mar.left());
    setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

    setAttribute(Qt::WA_AcceptTouchEvents);
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    const QRect BOUND =
        fontMetrics().boundingRect(0, 0, 360, 1000, Qt::TextWordWrap, document()->toPlainText());
    setMinimumHeight(std::max(fontMetrics().height(), BOUND.height()) +
                     Style::valueDPIScale(8) * 2);
}

void FlatPlainTextEdit::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter p(this->viewport());
    QColor   inputField(0x32, 0x32, 0x32);
    inputField = inputField.darker(175);
    p.setBrush(inputField);
    p.setPen(Qt::NoPen);
    p.drawRect(QRectF(0, 0, width(), height()).marginsRemoved(QMarginsF(2, 2, 2, 2)));

    QTextEdit::paintEvent(event);
}
