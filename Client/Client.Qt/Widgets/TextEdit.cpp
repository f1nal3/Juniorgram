#include "TextEdit.hpp"

#include <QtEvents>
#include <Style/Styles.hpp>

TextEdit::TextEdit(QWidget* parent) : QWidget(parent), _settings(Settings::getInstance())
{
    _mainVerticalLayout     = std::make_unique<QVBoxLayout>(this);
    _horizontalButtonLayout = std::make_unique<QHBoxLayout>();
    _boldnessButton         = std::make_unique<FlatButton>(this, "B", st::boldnessButton);
    _italicButton           = std::make_unique<FlatButton>(this, "I", st::italicButton);
    _underlineButton        = std::make_unique<FlatButton>(this, "U", st::underlineButton);
    _sendButton             = std::make_unique<FlatButton>(this, "Send");
    _messageInput           = std::make_unique<FlatTextEdit>();

    _sendMessage  = std::make_unique<QShortcut>
    (
        QKeySequence{ Qt::CTRL | Qt::Key_Return },
        this,
        [this]() { sendButtonClick(); }
    );

    _horizontalButtonSpacer = std::make_unique<QSpacerItem>(40, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalButtonLayout->setAlignment(Qt::AlignLeft);
    _horizontalButtonLayout->addWidget(_boldnessButton.get());
    _horizontalButtonLayout->addWidget(_italicButton.get());
    _horizontalButtonLayout->addWidget(_underlineButton.get());
    _horizontalButtonLayout->addItem(_horizontalButtonSpacer.get());
    _horizontalButtonLayout->addWidget(_sendButton.get());
    if (auto fontSize = _settings.getFontSize())
    {
        if (fontSize.has_value()) _messageInput->setFontPointSize(fontSize.value());
    }
    _mainVerticalLayout->addWidget(_messageInput.get());
    _mainVerticalLayout->addLayout(_horizontalButtonLayout.get());

    setLayout(_mainVerticalLayout.get());

    _boldnessButton->setClickCallback([&]() { styleButtonClick(_boldSymbolOpen, _boldSymbolClose); });
    _italicButton->setClickCallback([&]() { styleButtonClick(_italicSymbolOpen, _italicSymbolClose); });
    _underlineButton->setClickCallback([&]() { styleButtonClick(_underlineSymbolOpen, _underlineSymbolClose); });
    _sendButton->setClickCallback([&]() { sendButtonClick(); });
    
    connect(_messageInput.get(), &FlatTextEdit::textChanged, this, &TextEdit::textChanged);
    setMaximumHeight(Style::valueDPIScale(400));
    setMinimumHeight(Style::valueDPIScale(100));
}

int TextEdit::expectedHeight()
{
    return _boldnessButton->height() + _messageInput->document()->size().height() + _mainVerticalLayout->margin() +
           _messageInput->contentsMargins().top() + _messageInput->contentsMargins().bottom() + contentsMargins().top() +
           contentsMargins().bottom();
}

void TextEdit::sendButtonClick()
{
    if (getText() != "")
    {
        emit sendMessage(getText());
        clear();
    }
}

void TextEdit::styleButtonClick(const QString& symbolStart, const QString& symbolEnd)
{
    QTextCursor cursor = _messageInput->textCursor();

    if (cursor.hasSelection())
    {
        int start = cursor.selectionStart();
        int end   = cursor.selectionEnd();

        QString selectedText = cursor.selectedText();
        QString fullText     = getText();

        QString beforeSelectedText = fullText.left(start);
        QString afterSelectedText  = fullText.right(fullText.size() - end);

        if (selectedText.endsWith(symbolEnd) && selectedText.startsWith(symbolStart))
        {
            delSymbolsInSelection(fullText, start, end, _symbolSize);
            _messageInput->setTextCursor(cursor);
        }
        else if (beforeSelectedText.endsWith(symbolStart) && afterSelectedText.startsWith(symbolEnd))
        {
            delSymbolsOutSelection(fullText, start, end, _symbolSize);
            _messageInput->setTextCursor(cursor);
        }
        else
        {
            insertSymbolsInSelection(cursor, start, end, _symbolSize, symbolStart, symbolEnd);
            selectText(cursor, start, end);
        }
    }
}

void TextEdit::delSymbolsInSelection(QString& text, int& start, int& end, int symbolSize)
{
    text.replace(end - (symbolSize + 1), symbolSize + 1, "");
    text.replace(start, symbolSize, "");
    _messageInput->setPlainText(text);
}

void TextEdit::delSymbolsOutSelection(QString& text, int& start, int& end, int symbolSize)
{
    text.replace(end, symbolSize + 1, "");
    text.replace(start - symbolSize, symbolSize, "");
    _messageInput->setPlainText(text);
}

void TextEdit::insertSymbolsInSelection(QTextCursor& cursor, int& start, int& end, int symbolSize, const QString& symbolStart,
                                        const QString& symbolEnd)
{
    cursor.setPosition(start);
    cursor.insertText(symbolStart);
    end += symbolSize;

    cursor.setPosition((end));
    cursor.insertText(symbolEnd);
    end += symbolSize + 1;
}

void TextEdit::selectText(QTextCursor& cursor, int start, int end)
{
    cursor.setPosition(start, QTextCursor::MoveAnchor);
    cursor.setPosition(end, QTextCursor::KeepAnchor);
    _messageInput->setTextCursor(cursor);
}

QString TextEdit::getText() const { return _messageInput->toPlainText(); }

void TextEdit::clear() { _messageInput->clear(); }

TextEdit::~TextEdit() { _horizontalButtonLayout->removeItem(_horizontalButtonSpacer.get()); }
