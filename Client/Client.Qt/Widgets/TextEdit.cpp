#include "TextEdit.hpp"

#include <QtEvents>

#include <Style/Style.hpp>

TextEdit::TextEdit(QWidget* parent) : QWidget(parent)
{
    _mainVerticalLayout      = std::make_unique<QVBoxLayout>(this);
    _horizontalButtonLayout  = std::make_unique<QHBoxLayout>();
    _boldnessButton          = std::make_unique<FlatButton>(this, "B", st::boldnessButton);
    _italicButton            = std::make_unique<FlatButton>(this, "I", st::italicButton);
    _underlineButton         = std::make_unique<FlatButton>(this, "U", st::underlineButton);
    _sendButton              = std::make_unique<FlatButton>(this, "Send");
    _messageInput            = std::make_unique<FlatTextEdit>();
    _horizontalButtonSpacer  =
        std::make_unique<QSpacerItem>(40, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalButtonLayout->setAlignment(Qt::AlignLeft);
    _horizontalButtonLayout->addWidget(_boldnessButton.get());
    _horizontalButtonLayout->addWidget(_italicButton.get());
    _horizontalButtonLayout->addWidget(_underlineButton.get());
    _horizontalButtonLayout->addItem(_horizontalButtonSpacer.get());
    _horizontalButtonLayout->addWidget(_sendButton.get());
    _mainVerticalLayout->addWidget(_messageInput.get());
    _mainVerticalLayout->addLayout(_horizontalButtonLayout.get());
    setLayout(_mainVerticalLayout.get());
    connectUi();
}

void TextEdit::connectUi()
{
    _boldnessButton->setClickCallback([&]() { boldButtonClicked(_boldSymbolOpen, _boldSymbolClose); });
    _italicButton->setClickCallback(
        [&]() { boldButtonClicked(_italicSymbolOpen, _italicSymbolClose); });
    _underlineButton->setClickCallback(
        [&]() { boldButtonClicked(_underlineSymbolOpen, _underlineSymbolClose); });
    _sendButton->setClickCallback([&]() { clickButtonSend(); });
}

void TextEdit::clickButtonSend() { 
    if (getText() != "")
    {
        emit sendMessageSignal(getText());
        clearTextEdit();
    }
}

void TextEdit::keyPressEvent(QKeyEvent* event)
{
    if ((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) && (getText() != ""))
    {
        clickButtonSend();
    }
}

void TextEdit::boldButtonClicked(QString SymbolStart, QString SymbolEnd)
{
    QTextCursor cursor = _messageInput->textCursor();

    if (cursor.hasSelection())
    {
        int start = cursor.selectionStart();
        int end   = cursor.selectionEnd();

        QString mSelection = cursor.selectedText();
        QString mFullText  = getText();

        QString mBeforeSelection = mFullText.left(start);
        QString mAfterSelection  = mFullText.right(mFullText.size() - end);

        if (mSelection.endsWith(SymbolEnd) && mSelection.startsWith(SymbolStart))
        {
            delSymbolsInSelection(mFullText, start, end, SymbolSize);
            _messageInput->setTextCursor(cursor);
        }
        else if (mBeforeSelection.endsWith(SymbolStart) && mAfterSelection.startsWith(SymbolEnd))
        {
            delSymbolsOutSelection(mFullText, start, end, SymbolSize);
            _messageInput->setTextCursor(cursor);
        }
        else
        {
            insertSymbolsInSelection(cursor, start, end, SymbolSize, SymbolStart, SymbolEnd);
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

void TextEdit::insertSymbolsInSelection(QTextCursor& cursor, int& start, int& end, int symbolSize,
                                        const QString symbolStart, const QString symbolEnd)
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

QString TextEdit::getText() const
{
    return _messageInput->toPlainText();
}

void TextEdit::clearTextEdit() { _messageInput->clear(); }

TextEdit::~TextEdit()
{ _horizontalButtonLayout->removeItem(_horizontalButtonSpacer.get());
}
