#include "SimpleReplyWidget.hpp"

SimpleReplyWidget::SimpleReplyWidget(QWidget* history, QString message, uint64_t messageId, QString username,
                                     const Style::MessageWidget& st)
    : QWidget(history),
      _messageId(messageId),
      _messageText(std::move(message)),
      _username(std::move(username)),
      _st(st)
{
    setContentsMargins(QMargins(_st.radius, _st.radius, _st.radius, _st.radius));
    setMinimumHeight(_st.fontname->height + _st.radius * 2);

    _fmtMessageText = std::make_unique<FlatTextEdit>(this, _st.textedit);
    _fmtMessageText->setText(_messageText);
    _fmtMessageText->setAcceptDrops(false);
    _fmtMessageText->setReadOnly(true);
    _fmtMessageText->moveCursor(QTextCursor::Start);
    _fmtMessageText->setFont(_st.fonttext);
    _fmtMessageText->move(_st.radius * 2, _st.fontname->height + _st.radius * 4);
    _fmtMessageText->show();

    resize(width(), expectedHeight());
}

int SimpleReplyWidget::expectedHeight() const
{
    if (!_fmtMessageText) return 0;
    return _st.radius * 7 + _st.fontname->height + _fmtMessageText->document()->size().height();
}

void SimpleReplyWidget::paintEvent(QPaintEvent* e)
{
    QPainter p(this);
    auto     margin = _st.radius;
    p.setPen(QPen(Qt::white, 2));
    p.setRenderHint(QPainter::Antialiasing);

    auto thisrect = rect().marginsRemoved(QMargins(1, 1, 2, 2));
    p.drawRoundedRect(thisrect, margin, margin);

    p.setFont(_st.fontname);

    auto usernameRect = QRect(margin * 2, margin * 2 + 1, _st.fontname->width(_username), _st.fontname->height);
    p.drawText(usernameRect, _username);

    p.setFont(_st.fontdate);

    QWidget::paintEvent(e);
}

void SimpleReplyWidget::resizeEvent(QResizeEvent* e)
{
    emit geometryChanged(e->size().height() - e->oldSize().height());

    _fmtMessageText->resize(width() - _st.radius * 4 - 1, _fmtMessageText->document()->size().height());

    if (expectedHeight() != height())
    {
        resize(width(), expectedHeight());
    }
    QWidget::resizeEvent(e);
}
