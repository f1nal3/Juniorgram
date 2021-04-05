#include "TextEdit.hpp"

#include <Style/Style.hpp>

TextEdit::TextEdit(QWidget* parent) : QWidget(parent)
{
    textField  = new FlatPlainTextEdit();
    boldButton = new FlatButton("B");

    auto* vLayout = new QVBoxLayout;
    auto* hLayout = new QHBoxLayout;

    hLayout->setAlignment(Qt::AlignLeft);
    hLayout->addWidget(boldButton);
    vLayout->addWidget(textField);
    vLayout->addLayout(hLayout);

    setLayout(vLayout);
    connect(boldButton, SIGNAL(clicked()), this, SLOT(boldButtonClicked()));
}

void TextEdit::boldButtonClicked()
{
    QTextCursor cursor = textField->textCursor();

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
    QString message = textField->toPlainText();
    return message;
}

void TextEdit::clear() { textField->clear(); }

TextEdit::~TextEdit()
{
    delete boldButton;
    delete textField;
}
