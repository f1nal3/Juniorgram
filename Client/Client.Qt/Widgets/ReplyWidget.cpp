#include "ReplyWidget.hpp"

#include <QPainter>
#include <QtEvents>

ReplyWidget::ReplyWidget(QString message, QString username, uint64_t messageId, const Style::MessageWidget& st)
    : _messageText(message), _username(username), _messageId(messageId), _st(st)
{
    _fmtMessageText = std::make_unique<FlatTextEdit>(this, _st.textedit);
    _fmtMessageText->setText(_messageText);
    _fmtMessageText->setAcceptDrops(false);
    _fmtMessageText->setReadOnly(true);
    _fmtMessageText->moveCursor(QTextCursor::Start);
    _fmtMessageText->setFont(_st.fonttext);
    _fmtMessageText->move(_st.radius * 2, _st.fontname->height + _st.radius * 4);
    _fmtMessageText->show();
    resize(width(), expectedHeight());
    _closeReplyButton = std::make_unique<FlatButton>(this, "Close", _st.button);
    _closeReplyButton->setClickCallback([&](){ closeReply(); });
    setAttribute(Qt::WA_DeleteOnClose);
    emit createReplySignal(this);
}

void ReplyWidget::closeReply()
{
    this->close();
}

int ReplyWidget::expectedHeight()
{
    if (!_fmtMessageText) return 0;
    return _st.radius * 6 + _st.fontname->height + _fmtMessageText->document()->size().height();
}

/*
void ReplyWidget::paintEvent(QPaintEvent* e)
{
    QPainter p(this);
    auto     margin = _st.radius;
    p.setPen(QPen(Qt::white, 2));
    p.setRenderHint(QPainter::Antialiasing);

    auto thisrect = rect().marginsRemoved(QMargins(1, 1, 2, 2));
    p.drawRoundedRect(thisrect, margin, margin);

    p.setFont(_st.fontdate);

    QWidget::paintEvent(e);
}

void ReplyWidget::resizeEvent(QResizeEvent* e)
{
    emit geometryChanged(e->size().height() - e->oldSize().height());
    int closeReplyButtonX = width() - _st.radius - _closeReplyButton->width();
    _closeReplyButton->move(closeReplyButtonX, _st.radius);
    _fmtMessageText->resize(width() - _st.radius * 4 - 1, _fmtMessageText->document()->size().height());

    if (expectedHeight() != height())
    {
        resize(width(), expectedHeight());
    }
    QWidget::resizeEvent(e);
}
*/
