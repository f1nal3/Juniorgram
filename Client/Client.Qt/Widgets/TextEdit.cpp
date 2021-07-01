#include "TextEdit.hpp"

#include <QtEvents>

#include <Style/Style.hpp>

TextEdit::TextEdit(QWidget* parent)
    : QWidget(parent)
{

    mainVerticalLayout       = new QVBoxLayout(this);
    horizontaltButtonsLayout = new QHBoxLayout();
    mBoldButton              = new FlatButton(this, "B", st::boldnessButton);
    mItalicsButton           = new FlatButton(this, "I", st::italicButton);
    mUnderscoreButton        = new FlatButton(this, "U", st::underlineButton);
    sendButton               = new FlatButton(this, "Send");
    messageTextEdit          = new FlatPlainTextEdit();
    horizontalButtonSpacer   = new QSpacerItem(40, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontaltButtonsLayout->setAlignment(Qt::AlignLeft);
    horizontaltButtonsLayout->addWidget(mBoldButton);
    horizontaltButtonsLayout->addWidget(mItalicsButton);
    horizontaltButtonsLayout->addWidget(mUnderscoreButton);
    horizontaltButtonsLayout->addItem(horizontalButtonSpacer);
    horizontaltButtonsLayout->addWidget(sendButton);
    mainVerticalLayout->addWidget(messageTextEdit);
    mainVerticalLayout->addLayout(horizontaltButtonsLayout);
    setLayout(mainVerticalLayout);
    connectUi();
}

void TextEdit::connectUi()
{
    mBoldButton->setClickCallback([&]() { boldButtonClicked(boldSymbolStart, boldSymbolEnd); });
    mItalicsButton->setClickCallback(
        [&]() { boldButtonClicked(italicsSymbolStart, italicsboldSymbolEnd); });
    mUnderscoreButton->setClickCallback(
        [&]() { boldButtonClicked(underscoreSymbolStart, underscoreSymbolEnd); });
    sendButton->setClickCallback([&]() { clickButtonSend(); });
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
    QTextCursor cursor = messageTextEdit->textCursor();

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
            messageTextEdit->setTextCursor(cursor);
        }
        else if (mBeforeSelection.endsWith(SymbolStart) && mAfterSelection.startsWith(SymbolEnd))
        {
            delSymbolsOutSelection(mFullText, start, end, SymbolSize);
            messageTextEdit->setTextCursor(cursor);
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
    messageTextEdit->setPlainText(text);
}

void TextEdit::delSymbolsOutSelection(QString& text, int& start, int& end, int symbolSize)
{
    text.replace(end, symbolSize + 1, "");
    text.replace(start - symbolSize, symbolSize, "");
    messageTextEdit->setPlainText(text);
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
    messageTextEdit->setTextCursor(cursor);
}

QString TextEdit::getText() const
{
    return messageTextEdit->toPlainText();
}

void TextEdit::clearTextEdit() { messageTextEdit->clear(); }

TextEdit::~TextEdit()
{
    delete mBoldButton;
    delete mItalicsButton;
    delete mUnderscoreButton;
    delete messageTextEdit;
    delete sendButton;
    horizontaltButtonsLayout->removeItem(horizontalButtonSpacer);
    delete horizontalButtonSpacer;
    delete horizontaltButtonsLayout;
    delete mainVerticalLayout;
}
