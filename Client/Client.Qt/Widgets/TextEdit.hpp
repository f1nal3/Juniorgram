#pragma once
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <memory>

#include "Buttons.hpp"
#include "InputFields.hpp"

/** @class TextEdit
 *  @brief Widget with a text edit field and an edit bar.\
 * The edit bar has a Bold button which makes selected text bold.
 */
class TextEdit : public QWidget
{
    Q_OBJECT
public:
    explicit TextEdit(QWidget* parent = nullptr);
    /**
     * @brief Method for getting text from the text edit field.
     * @return text edit field content.
     */
    [[nodiscard]] QString getText() const;
    /**
     * @brief Method for clearing text edit field.
     */
    void clearTextEdit();
    ~TextEdit() override;

private:
    std::unique_ptr<QVBoxLayout>  _mainVerticalLayout;
    std::unique_ptr<QHBoxLayout>  _horizontalButtonLayout;
    std::unique_ptr<FlatButton>   _boldnessButton;
    std::unique_ptr<FlatButton>   _italicButton;
    std::unique_ptr<FlatButton>   _underlineButton;
    std::unique_ptr<FlatButton>   _sendButton;
    std::unique_ptr<FlatTextEdit> _messageInput;
    std::unique_ptr<QSpacerItem>  _horizontalButtonSpacer;

    const int     SymbolSize            = 3;
    const QString _boldSymbolOpen       = "<B>";
    const QString _boldSymbolClose      = "</B>";
    const QString _italicSymbolOpen     = "<I>";
    const QString _italicSymbolClose    = "</I>";
    const QString _underlineSymbolOpen  = "<U>";
    const QString _underlineSymbolClose = "</U>";

    void delSymbolsInSelection(QString& text, int& start, int& end, int symbolSize);
    void delSymbolsOutSelection(QString& text, int& start, int& end, int symbolSize);
    void insertSymbolsInSelection(QTextCursor& cursor, int& start, int& end, int symbolSize, QString symbolStart, QString symbolEnd);
    void selectText(QTextCursor& cursor, int start, int end);
    void connectUi();
    void keyPressEvent(QKeyEvent* event) override;

signals:
    void sendMessageSignal(QString textMessage);

public slots:
    void clickButtonSend();

private slots:
    void boldButtonClicked(QString SymbolStart, QString SymbolEnd);
};
