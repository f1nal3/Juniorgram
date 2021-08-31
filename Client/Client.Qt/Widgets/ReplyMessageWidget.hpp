#pragma once

#include <QWidget>
#include <QPainter>
#include <QResizeEvent>
#include <memory>

#include "Widgets/InputFields.hpp"
#include "Widgets/ReactionLayout.hpp"

/**
 * @class ReplyMessageWidget
 * @brief Class for display reply
 */
class ReplyMessageWidget : public QWidget
{
    Q_OBJECT

public:
    /// Constructor for reply message widget
    ReplyMessageWidget(QWidget* history, QString message, uint64_t messageId, QString username, uint64_t userId = uint64_t(),
                      const Style::MessageWidget& st = st::defaultMessageWidget);

public:
    /// Possible height of message widget
    int expectedHeight() const;

signals:
    /// Signal for change reply message geometry
    void geometryChanged(int);


protected:
    /// Event that is responsible for rendering the message
    void paintEvent(QPaintEvent* e) override;
    /// Event that is responsible for changing the size of the message
    void resizeEvent(QResizeEvent* e) override;

private:
    std::unique_ptr<FlatTextEdit>   _fmtMessageText;
    uint64_t                        _messageId;
    QString                         _messageText;
    QString                         _username;
    uint64_t                        _userId;
    const Style::MessageWidget&     _st;
    const QString                   _replyTag = "Reply";
};
