#include "TextEdit.hpp"

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

        cursor.setPosition(start);
        cursor.insertText(QString("**"));

        cursor.setPosition((end + 2));
        cursor.insertText(QString("**"));
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
