#include "ReplyMessageWidget.hpp"

ReplyMessageWidget::ReplyMessageWidget(QWidget* history, QString message, uint64_t messageId, QString username, uint64_t userId,
                                       const Style::MessageWidget& st)
    : AbstractMessageWidget(history, message, messageId, username, userId, st)
{
    
}

int ReplyMessageWidget::expectedHeight() const
{
    if (!getFmtMessageText()) return 0;
    return static_cast<int>(getStyle().radius * 7 + getStyle().fontname->height + getFmtMessageText()->document()->size().height());
}

void ReplyMessageWidget::paintEvent(QPaintEvent* e)
{
    QPainter p(this);
    auto     margin = getStyle().radius;
    p.setPen(QPen(Qt::white, 2));
    p.setRenderHint(QPainter::Antialiasing);

    auto thisrect = rect().marginsRemoved(QMargins(1, 1, 2, 2));
    p.drawRoundedRect(thisrect, margin, margin);

    p.setFont(getStyle().fontname);

    auto usernameRect = QRect(margin * 2, margin * 2 + 1, getStyle().fontname->width(getUserName()), getStyle().fontname->height);
    p.drawText(usernameRect, getUserName());

    auto replyRect = QRect(margin * 2, margin * 2 + 1, getStyle().fontname->width(_replyTag), getStyle().fontname->height);
    replyRect.moveRight(width() - getStyle().radius - 10);
    p.drawText(replyRect, _replyTag);

    QWidget::paintEvent(e);
}

void ReplyMessageWidget::resizeEvent(QResizeEvent* e)
{
    emit geometryChanged(e->size().height() - e->oldSize().height());

    getFmtMessageText()->resize(width() - getStyle().radius * 4 - 1, static_cast<int>(getFmtMessageText()->document()->size().height()));

    if (expectedHeight() != height())
    {
        resize(width(), expectedHeight());
    }
    QWidget::resizeEvent(e);
}
