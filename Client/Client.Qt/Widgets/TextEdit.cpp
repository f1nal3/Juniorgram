#include "TextEdit.hpp"
#include <iostream>

#include <Style/Style.hpp>

TextEdit::TextEdit(QWidget* parent) : QWidget(parent)
{
    mTextField  = new FlatPlainTextEdit();
    mBoldButton = new FlatButton("B");

    auto* vLayout = new QVBoxLayout;
    auto* hLayout = new QHBoxLayout;

    hLayout->setAlignment(Qt::AlignLeft);
    hLayout->addWidget(mBoldButton);

    vLayout->addWidget(mTextField);
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

        if(mSelection.endsWith("**") && mSelection.startsWith("**"))
        {
            mFullText.replace(end - 2, 2, QString(""));
            mFullText.replace(start, 2, QString(""));
            mTextField->setPlainText(mFullText);
            mTextField->setTextCursor(cursor);
        }
        else
        {
        cursor.setPosition(start);
        cursor.insertText(QString("**"));
        end += boldSymbolSize;

        cursor.setPosition((end));
        cursor.insertText(QString("**"));
        end += boldSymbolSize;

        cursor.setPosition(start, QTextCursor::MoveAnchor);
        cursor.setPosition(end, QTextCursor::KeepAnchor);
        mTextField->setTextCursor(cursor);
        }
    }
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
    delete mTextField;
}
