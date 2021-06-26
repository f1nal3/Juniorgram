#pragma once
#include "FlatButton.hpp"
#include "InputFields.hpp"
#include "pch.hpp"

/** @class TextEdit
 *  @brief Widget with a text edit field and an edit bar.\
 * The edit bar has a Bold button which makes selected text bold.
 */
class TextEdit : public QWidget
{
    Q_OBJECT
public:
    TextEdit(FlatPlainTextEdit** thisMessageTextEdit, FlatButton** thisSendButton,
             QWidget* parent = nullptr);
    /**
     * @brief Method for getting text from the text edit field.
     * @return text edit field content as QString.
     */
    [[nodiscard]] QString getText() const;
    /**
     * @brief Method for clearing text edit field.
     */
    void clear();
    ~TextEdit() override;

private:
    QVBoxLayout* mainVerticalLayout;
    QHBoxLayout* horizontaltLayout;

    FlatButton* mBoldButton;
    FlatButton* mItalicsButton;
    FlatButton* mUnderscoreButton;
    FlatButton* sendButton;
    FlatPlainTextEdit* messageTextEdit;
    QSpacerItem* horizontalButtonSpacer;

    const int SymbolSize                = 3;
    const QString boldSymbolStart       = "<B>";
    const QString boldSymbolEnd         = "</B>";
    const QString italicsSymbolStart    = "<I>";
    const QString italicsboldSymbolEnd  = "</I>";
    const QString underscoreSymbolStart = "<U>";
    const QString underscoreSymbolEnd   = "</U>";

    void delSymbolsInSelection(QString& text, int& start, int& end, int symbolSize);
    void delSymbolsOutSelection(QString& text, int& start, int& end, int symbolSize);
    void insertSymbolsInSelection(QTextCursor& cursor, int& start, int& end, int symbolSize,
                                  const QString symbolStart, const QString symbolEnd);
    void selectText(QTextCursor& cursor, int start, int end);
    void TextEdit::connectUi();

private slots:
    void boldButtonClicked(QString SymbolStart, QString SymbolEnd);
};
