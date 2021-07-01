#include "TextEdit.hpp"

#include <Style/Style.hpp>

TextEdit::TextEdit(QListWidget* thisChatWidget, QWidget* parent)
    : QWidget(parent), chatWidget(thisChatWidget)
{
    mainVerticalLayout       = new QVBoxLayout(this);
    horizontaltButtonsLayout = new QHBoxLayout();
    mBoldButton              = new FlatButton(this, "B", st::boldnessButton);
    mItalicsButton           = new FlatButton(this, "I", st::boldnessButton);
    mUnderscoreButton        = new FlatButton(this, "U", st::boldnessButton);
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
    sendButton->setClickCallback([&]() { updateMessagesList_User(); });
}

void TextEdit::updateMessagesList_User()
{
    if (getText() != "") newMessage(getText());
    messageTextEdit->clear();
}

void TextEdit::keyPressEvent(QKeyEvent* event)
{
    if ((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) &&
        (getText() != ""))
    {
        newMessage(getText());
        messageTextEdit->clear();
    }
}

 void TextEdit::newMessage(QString textMessage, QString userNameMessage)
{
    auto* item = new QListWidgetItem();
    item->setSizeHint(QSize(0, Style::valueDPIScale(150)));
    auto* myItem =
        new MessageWidget(std::move(textMessage), std::move(userNameMessage), item, false);
    myItem->setThisItem(item);
    chatWidget->addItem(item);
    chatWidget->setItemWidget(item, myItem);
}

void TextEdit::newMessage(QString textMessage)
{
    auto* item = new QListWidgetItem();
    item->setSizeHint(QSize(0, Style::valueDPIScale(150)));
    auto* myItem = new MessageWidget(std::move(textMessage), item, false);
    myItem->setThisItem(item);
    chatWidget->addItem(item);
    chatWidget->setItemWidget(item, myItem);
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

void TextEdit::clear() { messageTextEdit->clear(); }

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
