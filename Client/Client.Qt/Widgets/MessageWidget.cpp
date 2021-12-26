#include "MessageWidget.hpp"

#include <QDebug>
#include <QPainter>
#include <QtEvents>
#include <Style/Styles.hpp>
#include <utility>

#include "Application.hpp"
#include "PopupWidget.hpp"
#include "Widgets/ChatHistory.hpp"
#include "Widgets/ReactionLayout.hpp"

MessageWidget::MessageWidget(QWidget* history, QString message, uint64_t userId, uint64_t messageId, qint64 utc, QString username,
                             const Style::MessageWidget& st)
    : AbstractMessageWidget(history, message, messageId, username, userId, st) 
    , _datetime(QDateTime::fromMSecsSinceEpoch(utc))
{   
    _menuBtn = std::make_unique<FlatButton>(this, "Menu", getStyle().button);
    _menuBtn->setClickCallback([=]() {
        auto popup = new PopupWidget();
        popup->setDeleteOnHide(true);
        auto       messageMenu   = std::make_unique<Menu>();
        const auto menuReactions = new ReactionLayout(this, 400, 0, true);

        connect(menuReactions, &ReactionLayout::onClick,
                this, &MessageWidget::onReaction);

        messageMenu->addAction(menuReactions);
        messageMenu->addSeparator();

        // T\todo implement a better way to delete a message "through server"
        messageMenu->addAction("Delete message", [=]() { onDelete(); });
        popup->setMenu(std::move(messageMenu));

        auto globalPoint = mapToGlobal(QPoint(_menuBtn->x(), _menuBtn->height()));
        popup->popup(QPoint(globalPoint.x(), globalPoint.y() + 1));
    });

    _reactions = std::make_unique<ReactionLayout>(this);
    
    connect(_reactions.get(), &ReactionLayout::onClick,
            this, &MessageWidget::onReaction);

    _replyBtn = std::make_unique<FlatButton>(this, "Reply", getStyle().button);
    _replyBtn->setClickCallback([&](){ createReply(); });

    resize(width(), expectedHeight());
}

void MessageWidget::paintEvent(QPaintEvent* e)
{
    QPainter p(this);
    auto     margin = getStyle().radius;
    p.setPen(QPen(Qt::white, 2));
    p.setRenderHint(QPainter::Antialiasing);

    auto thisrect = rect().marginsRemoved(QMargins(1, 1, 2, 2));
    p.drawRoundedRect(thisrect, margin, margin);

    p.setFont(getStyle().fontname);
    if (_messageFlags & MessageFlag::Deleted)
    {
        QString dltmsg = QString("Message was deleted");
        p.drawText(QRect(margin, margin, getStyle().fontname->width(dltmsg), getStyle().fontname->height), dltmsg, Style::al_center);
        return;
    }

    auto usernameRect = QRect(margin * 2, margin * 2 + 1, getStyle().fontname->width(getUserName()), getStyle().fontname->height);
    p.drawText(usernameRect, getUserName());

    p.setFont(getStyle().fontdate);
    QString datetime     = _datetime.toString("d.MM hh:mm");
    auto    datetimeRect = QRect(margin * 3 + usernameRect.right(), 
                                 margin * 2 + 1,
                                 getStyle().fontdate->width(datetime), 
                                 getStyle().fontdate->height);
    p.drawText(datetimeRect, datetime, Style::al_center);

    QWidget::paintEvent(e);
}

void MessageWidget::resizeEvent(QResizeEvent* e)
{
    emit geometryChanged(e->size().height() - e->oldSize().height());
    if (_messageFlags & MessageFlag::Deleted) return QWidget::resizeEvent(e);
    _reactions->recountSize();
    int menuButtonX = width() - getStyle().radius - _menuBtn->width();
    _menuBtn->move(menuButtonX, getStyle().radius);
    int replyButtonX = width() - getStyle().radius - _menuBtn->width() - _replyBtn->width() - 2;
    _replyBtn->move(replyButtonX, getStyle().radius);
    getFmtMessageText()->resize(width() - getStyle().radius * 4 - 1, getFmtMessageText()->document()->size().height());
    _reactions->move(getStyle().radius * 2, getFmtMessageText()->y() + getFmtMessageText()->document()->size().height() + getStyle().radius);

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
    resize(width(), getStyle().fontname->height + getStyle().radius * 2);
    update();
    oApp->connectionManager()->userMessageDelete(getMessageID());
}

void MessageWidget::onReaction(const std::uint32_t reactionID)
{
    oApp->connectionManager()->userMessageReaction(getMessageID(), reactionID);
}

void MessageWidget::setMessageText(const QString& newMessage)
{
    if (newMessage != "")
    {
        AbstractMessageWidget::setMessageText(newMessage);
        update();
    }
}

void MessageWidget::setUserName(const QString& newUserName)
{
    if (newUserName != "")
    {
        AbstractMessageWidget::setUserName(newUserName);
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
    getFmtMessageText()->hide();
    _menuBtn->hide();
    _replyBtn->hide();
}

int MessageWidget::expectedHeight() const
{
    if (!getFmtMessageText()) return 0;
    if (_messageFlags & MessageFlag::Deleted)
    {
        return getStyle().fontname->height + getStyle().radius * 2;
    }
    return getStyle().radius * 7 + getStyle().fontname->height + getFmtMessageText()->document()->size().height() + _reactions->layoutSize().height();
}

void MessageWidget::createReply() { emit createReplySignal(getMessageText(), getUserName(), getMessageID()); }
