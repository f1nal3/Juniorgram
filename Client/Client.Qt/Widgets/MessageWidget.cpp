#include "MessageWidget.hpp"

#include <QDebug>
#include <QPainter>
#include <QtEvents>
#include <utility>

#include "ConnectionManager.hpp"
#include "Style/Style.hpp"
#include "Widgets/ChatHistory.hpp"
#include "Widgets/ReactionLayout.hpp"

MessageWidget::MessageWidget(QWidget* history, QString message, uint64_t userId, uint64_t messageId, qint64 utc, QString username,
                             const Style::MessageWidget& st)
    : QWidget(history),
      _userId(userId),
      _messageId(messageId),
      _messageText(std::move(message)),
      _username(std::move(username)),
      _datetime(QDateTime::fromSecsSinceEpoch(utc)),
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
    _menuBtn = std::make_unique<FlatButton>(this, "Menu", _st.button);
    _menuBtn->setClickCallback([=]() {
        auto popup = new PopupWidget();
        popup->setDeleteOnHide(true);
        auto       messageMenu   = std::make_unique<Menu>();
        const auto menuReactions = new ReactionLayout(this, 400, 0, true);
        messageMenu->addAction(menuReactions);
        messageMenu->addSeparator();

        // TODO: implement a better way to delete a message "through server"
        messageMenu->addAction("Delete message", [=]() { onDelete(); });
        popup->setMenu(std::move(messageMenu));

        auto globalPoint = mapToGlobal(QPoint(_menuBtn->x(), _menuBtn->height()));
        popup->popup(QPoint(globalPoint.x(), globalPoint.y() + 1));
    });
    _reactions = std::make_unique<ReactionLayout>(this);
    std::map<uint32_t, uint32_t> model;
    model.insert({1, 16});
    model.insert({2, 15});
    model.insert({0, 3});
    _reactions->setReactionsModel(model);
    resize(width(), expectedHeight());
}

void MessageWidget::paintEvent(QPaintEvent* e)
{
    QPainter p(this);
    auto     margin = _st.radius;
    p.setPen(QPen(Qt::white, 2));
    p.setRenderHint(QPainter::Antialiasing);

    auto thisrect = rect().marginsRemoved(QMargins(1, 1, 2, 2));
    p.drawRoundedRect(thisrect, margin, margin);

    p.setFont(_st.fontname);
    if (_messageFlags & MessageFlag::Deleted)
    {
        QString dltmsg = QString("Message was deleted");
        p.drawText(QRect(margin, margin, _st.fontname->width(dltmsg), _st.fontname->height), dltmsg, Style::al_center);
        return;
    }

    auto usernameRect = QRect(margin * 2, margin * 2 + 1, _st.fontname->width(_username), _st.fontname->height);
    p.drawText(usernameRect, _username);

    p.setFont(_st.fontdate);
    QString datetime     = _datetime.toString("d.MM hh:mm");
    auto    datetimeRect = QRect(margin * 3 + usernameRect.right(), margin * 2 + 1, _st.fontdate->width(datetime), _st.fontdate->height);
    p.drawText(datetimeRect, datetime, Style::al_center);

    QWidget::paintEvent(e);
}

void MessageWidget::resizeEvent(QResizeEvent* e)
{
    geometryChanged(e->size().height() - e->oldSize().height());
    if (_messageFlags & MessageFlag::Deleted) return QWidget::resizeEvent(e);
    _reactions->recountSize();
    int deleteButtonX = width() - _st.radius - _menuBtn->width();
    _menuBtn->move(deleteButtonX, _st.radius);
    _fmtMessageText->resize(width() - _st.radius * 4 - 1, _fmtMessageText->document()->size().height());
    _reactions->move(_st.radius * 2, _fmtMessageText->y() + _fmtMessageText->document()->size().height() + _st.radius);

    if (expectedHeight() != height())
    {
        resize(width(), expectedHeight());
    }
    QWidget::resizeEvent(e);
}

void MessageWidget::onDelete()
{
    clearMessage();
    _messageFlags |= MessageFlag::Deleted;
    resize(width(), _st.fontname->height + _st.radius * 2);
    update();
    ConnectionManager::getClient().messageUserDelete(_userId, _messageId);
}

void MessageWidget::setMessageText(const QString& newMessage)
{
    if (newMessage != "")
    {
        _messageText = newMessage;
        update();
    }
}

void MessageWidget::setUserName(const QString& newUserName)
{
    if (newUserName != "")
    {
        _username = newUserName;
        update();
    }
}

void MessageWidget::setTime(qint64 utc)
{
    _datetime.setSecsSinceEpoch(utc);
    update();
}

void MessageWidget::setReactionMap(const std::map<uint32_t, uint32_t>& newReactionMap)
{
    _reactions->setReactionsModel(newReactionMap);
    update();
}

void MessageWidget::clearMessage()
{
    _fmtMessageText->hide();
    _menuBtn->hide();
}

int MessageWidget::expectedHeight() const
{
    if (!_fmtMessageText) return 0;
    if (_messageFlags & MessageFlag::Deleted)
    {
        return _st.fontname->height + _st.radius * 2;
    }
    return _st.radius * 7 + _st.fontname->height + _fmtMessageText->document()->size().height() + _reactions->layoutSize().height();
}
