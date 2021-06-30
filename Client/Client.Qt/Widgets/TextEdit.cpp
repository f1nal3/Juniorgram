#include "TextEdit.hpp"

#include <Style/Style.hpp>

TextEdit::TextEdit(FlatPlainTextEdit* messageText, QWidget* parent) : QWidget(parent)
{
    mTextField        = messageText;
    mBoldButton       = std::make_unique<FlatButton>(this, "B", st::boldnessButton);
    mItalicsButton    = std::make_unique<FlatButton>(this, "I", st::italicButton);
    mUnderscoreButton = std::make_unique<FlatButton>(this, "U", st::underlineButton);

    vLayout = std::make_unique<QVBoxLayout>();
    hLayout = std::make_unique<QHBoxLayout>;

    hLayout->setAlignment(Qt::AlignLeft);
    hLayout->addWidget(mBoldButton.get());
    hLayout->addWidget(mItalicsButton.get());
    hLayout->addWidget(mUnderscoreButton.get());

    vLayout->addLayout(hLayout.get());

    setLayout(vLayout.get());
    mBoldButton->setClickCallback([&]() { boldButtonClicked(boldSymbolStart, boldSymbolEnd); });
    mItalicsButton->setClickCallback(
        [&]() { boldButtonClicked(italicsSymbolStart, italicsboldSymbolEnd); });
    mUnderscoreButton->setClickCallback(
        [&]() { boldButtonClicked(underscoreSymbolStart, underscoreSymbolEnd); });
}

void TextEdit::boldButtonClicked(QString SymbolStart, QString SymbolEnd)
{
    QTextCursor cursor = mTextField->textCursor();

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
            mTextField->setTextCursor(cursor);
        }
        else if (mBeforeSelection.endsWith(SymbolStart) && mAfterSelection.startsWith(SymbolEnd))
        {
            delSymbolsOutSelection(mFullText, start, end, SymbolSize);
            mTextField->setTextCursor(cursor);
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
    mTextField->setPlainText(text);
}

void TextEdit::delSymbolsOutSelection(QString& text, int& start, int& end, int symbolSize)
{
    text.replace(end, symbolSize + 1, "");
    text.replace(start - symbolSize, symbolSize, "");
    mTextField->setPlainText(text);
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
    mTextField->setTextCursor(cursor);
}

QString TextEdit::getText() const
{
    return mTextField->toPlainText();
}

void TextEdit::clear() { mTextField->clear(); }
