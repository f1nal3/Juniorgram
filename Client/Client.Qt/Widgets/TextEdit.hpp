#pragma once
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <memory>

#include "Buttons.hpp"
#include "InputFields.hpp"
#include "Settings.hpp"

/**
 *  @class TextEdit
 *  @brief Widget with a text edit field and an edit bar.\
 *  The edit bar has a Bold, Italics and Underscores which makes selected text, and the send button.
 */
class TextEdit : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for widget textEdit.
     * @param parent Parent widget
     */
    explicit TextEdit(QWidget* parent = nullptr);
    /**
     * @brief Method for getting text from the text edit field.
     * @return text edit field content.
     */
    [[nodiscard]] QString getText() const;

    int expectedHeight();

    /// clear text
    void clear();
    /// Destructor for text edit
    ~TextEdit() override;

public slots:
    /// send message
    void sendButtonClick();

signals:
    /// message send
    void sendMessage(QString textMessage);

private slots:
    void styleButtonClick(const QString& symbolStart, const QString& symbolEnd);

private:
    void delSymbolsInSelection(QString& text, int& start, int& end, int symbolSize);
    void delSymbolsOutSelection(QString& text, int& start, int& end, int symbolSize);
    void insertSymbolsInSelection(QTextCursor& cursor, int& start, int& end, int symbolSize, const QString& symbolStart,
                                  const QString& symbolEnd);
    void selectText(QTextCursor& cursor, int start, int end);

private:
    Settings& _settings;

    const int     _symbolSize           = 3;
    const QString _boldSymbolOpen       = "<B>";
    const QString _boldSymbolClose      = "</B>";
    const QString _italicSymbolOpen     = "<I>";
    const QString _italicSymbolClose    = "</I>";
    const QString _underlineSymbolOpen  = "<U>";
    const QString _underlineSymbolClose = "</U>";

    std::unique_ptr<QVBoxLayout>  _mainVerticalLayout;
    std::unique_ptr<QHBoxLayout>  _horizontalButtonLayout;
    std::unique_ptr<FlatButton>   _boldnessButton;
    std::unique_ptr<FlatButton>   _italicButton;
    std::unique_ptr<FlatButton>   _underlineButton;
    std::unique_ptr<FlatButton>   _sendButton;
    std::unique_ptr<FlatTextEdit> _messageInput;
    std::unique_ptr<QSpacerItem>  _horizontalButtonSpacer;
};
