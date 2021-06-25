#include "TextEdit.hpp"

#include <Style/Style.hpp>

TextEdit::TextEdit(FlatPlainTextEdit* messageText, QWidget* parent) : QWidget(parent)
{
    mTextField             = messageText;
    mBoldButton            = new FlatButton(this, "B", st::boldnessButton);
    mItalicsButton         = new FlatButton(this, "I", st::boldnessButton);
    mUnderscoreButton      = new FlatButton(this, "U", st::boldnessButton);
    sendButton             = new FlatButton(this, "Send");
    horizontalButtonSpacer = new QSpacerItem(40, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    vLayout = new QVBoxLayout;
    hLayout = new QHBoxLayout;

    hLayout->setAlignment(Qt::AlignLeft);
    hLayout->addWidget(mBoldButton);
    hLayout->addWidget(mItalicsButton);
    hLayout->addWidget(mUnderscoreButton);
    hLayout->addItem(horizontalButtonSpacer);
    hLayout->addWidget(sendButton);
    vLayout->addLayout(hLayout);

    setLayout(vLayout);
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

TextEdit::~TextEdit()
{
    delete mBoldButton;
    delete mItalicsButton;
    delete hLayout;
    delete vLayout;
}
