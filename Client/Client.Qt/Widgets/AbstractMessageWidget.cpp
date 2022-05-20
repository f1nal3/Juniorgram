#include "AbstractMessageWidget.hpp"

AbstractMessageWidget::AbstractMessageWidget(QWidget* history, QString message, uint64_t messageID, 
    QString username, uint64_t userID,
    const Style::MessageWidget& st)
    : QWidget(history)
    , _messageID(messageID)
    , _userID(userID)
    , _messageText(std::move(message))
    , _userName(std::move(username))
    , _st(st)
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
}
