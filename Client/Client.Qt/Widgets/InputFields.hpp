#pragma once

#include <QLineEdit>
#include <QTextEdit>
#include <Style/Styles.hpp>

/**
 * @brief Single line input field
 */
class FlatInput : public QLineEdit
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for input field
     * @param parent Parent widget
     * @param placeholder Text to show when nothing has entered
     * @param password Is this a password field?
     */
    explicit FlatInput(QWidget* parent = nullptr, const QString& placeholder = QString(), bool password = false);
    /**
     * @brief Inner size of input field
     * @return Inner size
     */
    [[nodiscard]] QRect getTextRect() const;

protected:
    /// Handle paint
    void paintEvent(QPaintEvent* event) override;
};

/**
 * @brief Multiline input field for inputs/messages
 */
class FlatTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for multiline input field
     * @param parent Parent widget
     * @param st Styles for field
     */
    explicit FlatTextEdit(QWidget* parent = nullptr, const Style::FlatTextEdit& st = st::defaultTextEdit);

protected:
    /// Handle paint
    void paintEvent(QPaintEvent* paintEvent) override;
    /// Handle resize
    void resizeEvent(QResizeEvent* resizeEvent) override;
    /// Handles pressing Shift+Enter combination
    void keyPressEvent(QKeyEvent* keyEvent) override;

signals:
    void shiftAndEnterPressed();
private:
    const Style::FlatTextEdit& _st;
};
