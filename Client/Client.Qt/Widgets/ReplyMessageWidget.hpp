#pragma once

#include <QPainter>
#include <QResizeEvent>
#include <QWidget>
#include <memory>

#include "Widgets/InputFields.hpp"
#include "Widgets/ReactionLayout.hpp"
#include "Widgets/AbstractMessageWidget.hpp"

/**
 * @class ReplyMessageWidget
 * @brief Class for display reply
 */
class ReplyMessageWidget : public AbstractMessageWidget
{
    Q_OBJECT

public:
    /// Constructor for reply message widget
    ReplyMessageWidget(QWidget* history, QString message, uint64_t messageId, QString username, uint64_t userId = uint64_t(),
                       const Style::MessageWidget& st = st::defaultMessageWidget);

    /// Possible height of message widget
    int expectedHeight() const override;

protected:
    /// Event that is responsible for rendering the message
    void paintEvent(QPaintEvent* e) override;
    /// Event that is responsible for changing the size of the message
    void resizeEvent(QResizeEvent* e) override;

private:
    const QString                 _replyTag = "Reply";
};
