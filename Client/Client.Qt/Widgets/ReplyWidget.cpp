#include "ReplyWidget.hpp"

#include <QPainter>
#include <QtEvents>
#include <utility>

ReplyWidget::ReplyWidget(QWidget* parent, const Style::MessageWidget& st) : QWidget(parent), _st(st)
{
    _closeReplyButton = std::make_unique<FlatButton>(this, "Close", _st.button);
    _closeReplyButton->setClickCallback([this]() { this->close(); });
    resize(width(), expectedHeight());
}

int ReplyWidget::expectedHeight() const{ return _st.radius * 5 + _st.fontname->height + _st.fonttext->height; }

void ReplyWidget::paintEvent(QPaintEvent* e)
{
    QPainter p(this);
    auto     margin = _st.radius;
    p.setPen(QPen(Qt::white, 2));
    p.setFont(_st.fontname);

    auto usernameRect = QRect(margin * 2, margin * 2 + 1, _st.fontname->width(_username), _st.fontname->height);
    p.drawText(usernameRect, _username);

    auto drawText = _st.fonttext->elided(_messageText, QWidget::width() - margin * 4);

    auto textRect = QRect(margin * 2, margin * 3 + 1 + _st.fontname->height, _st.fonttext->width(drawText), _st.fonttext->height);
    p.setFont(_st.fonttext);
    p.drawText(textRect, drawText);

    QWidget::paintEvent(e);
}

void ReplyWidget::resizeEvent(QResizeEvent* e)
{
    int closeReplyButtonX = width() - _st.radius * 2 - _closeReplyButton->width();
    _closeReplyButton->move(closeReplyButtonX, _st.radius);

    if (expectedHeight() != height())
    {
        resize(width(), expectedHeight());
    }

    resize(width(), expectedHeight());
    QWidget::resizeEvent(e);
}

void ReplyWidget::setReply(QString messageText, QString username, uint64_t messageId)
{
    _messageText = std::move(messageText);
    _username    = std::move(username);
    _messageId   = messageId;
    update();
}

void ReplyWidget::hideEvent(QHideEvent*) { visibilityChanged(true); }

void ReplyWidget::showEvent(QShowEvent*) { visibilityChanged(false); }
