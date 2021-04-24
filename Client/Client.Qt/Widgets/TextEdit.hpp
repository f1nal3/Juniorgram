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
    explicit TextEdit(FlatPlainTextEdit* messageText, FlatButton* sendButton, QWidget* parent = nullptr);
    /**
     * @brief Method for getting text from the text edit field.
     * @return text edit field content as QString.
     */
    [[nodiscard]] QString text() const;
    /**
     * @brief Method for clearing text edit field.
     */
    void clear();
    ~TextEdit() override;

private:
    QVBoxLayout* vLayout;
    QHBoxLayout* hLayout;

    FlatButton* mBoldButton;
    FlatPlainTextEdit* mTextField;

    QSpacerItem* spacerBetweenButton;
    
    const int boldSymbolSize = 2;
    const QString boldSymbol = "**";

    void delSymbolsInSelection(QString &text, int &start, int &end, int symbolSize);
    void delSymbolsOutSelection(QString &text, int &start, int &end, int symbolSize);
    void insertSymbolsInSelection(QTextCursor &cursor, int &start, int &end, int symbolSize, const QString symbol);
    void selectText(QTextCursor &cursor, int start, int end);

private slots:
    void boldButtonClicked();
};
