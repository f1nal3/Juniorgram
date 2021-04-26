#include "TextEdit.hpp"
#include <Style/Style.hpp>

TextEdit::TextEdit(FlatPlainTextEdit* messageText, QWidget* parent) : QWidget(parent)
{
    mTextField  = std::move(messageText);
    mBoldButton = new FlatButton("B");

    vLayout = new QVBoxLayout;
    hLayout = new QHBoxLayout;

    hLayout->setAlignment(Qt::AlignLeft);
    hLayout->addWidget(mBoldButton);

    vLayout->addLayout(hLayout);

    setLayout(vLayout);
    connect(mBoldButton, SIGNAL(clicked()), this, SLOT(boldButtonClicked()));
}

void TextEdit::boldButtonClicked()
{
    QTextCursor cursor = mTextField->textCursor();

    if (cursor.hasSelection())
    {
        int start = cursor.selectionStart();
        int end   = cursor.selectionEnd();

        QString mSelection = cursor.selectedText();
        QString mFullText = text();

        QString mBeforeSelection = mFullText.left(start);
        QString mAfterSelection = mFullText.right(mFullText.size() - end);  

        if(mSelection.endsWith(boldSymbol) && mSelection.startsWith(boldSymbol))
        {
            delSymbolsInSelection(mFullText, start, end, boldSymbolSize);
            mTextField->setTextCursor(cursor);
        }
        else if(mBeforeSelection.endsWith(boldSymbol) && mAfterSelection.startsWith(boldSymbol))
        {
            delSymbolsOutSelection(mFullText, start, end, boldSymbolSize);
            mTextField->setTextCursor(cursor);
        }
        else
        {
            insertSymbolsInSelection(cursor, start, end, boldSymbolSize, boldSymbol);
            selectText(cursor, start, end);
        }
    }
}

void TextEdit::delSymbolsInSelection(QString &text, int &start, int &end, int symbolSize){
    text.replace(end - symbolSize, symbolSize, "");
    text.replace(start, symbolSize, "");
    mTextField->setPlainText(text);
}

void TextEdit::delSymbolsOutSelection(QString &text, int &start, int &end, int symbolSize){
    text.replace(end, symbolSize, "");
    text.replace(start - symbolSize, symbolSize, "");
    mTextField->setPlainText(text);
}

void TextEdit::insertSymbolsInSelection(QTextCursor &cursor, int &start, int &end, int symbolSize, const QString symbol){
    cursor.setPosition(start);
    cursor.insertText(symbol);
    end += symbolSize;

    cursor.setPosition((end));
    cursor.insertText(symbol);
    end += symbolSize;
}

void TextEdit::selectText(QTextCursor &cursor, int start, int end){
    cursor.setPosition(start, QTextCursor::MoveAnchor);
    cursor.setPosition(end, QTextCursor::KeepAnchor);
    mTextField->setTextCursor(cursor);
}

QString TextEdit::text() const
{
    QString message = mTextField->toPlainText();
    return message;
}

void TextEdit::clear() { mTextField->clear(); }

TextEdit::~TextEdit()
{
    delete mBoldButton;
    //delete mTextField;
    delete hLayout;
    delete vLayout;
}
