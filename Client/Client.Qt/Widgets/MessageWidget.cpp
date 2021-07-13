#include "MessageWidget.hpp"

#include <QPainter>
#include <QtEvents>

#include "ChatHistory.hpp"
#include "Style/Style.hpp"

MessageWidget::MessageWidget(QWidget* history, QString message, qint64 utc, QString user, const Style::MessageWidget& st)
    : QWidget(history),
      _messageText(std::move(message)),
      _username(std::move(user)),
      dateTimeMessage(QDateTime::fromSecsSinceEpoch(utc)),
      _st(st)
{
    setContentsMargins(QMargins(_st.radius, _st.radius, _st.radius, _st.radius));
    reactionUserOnMessage = reactions::NON;
    setMinimumHeight(_st.fontname->height + _st.radius * 2);

    _fmtMessageText = std::make_unique<FlatTextEdit>(this, _st.textedit);
    _fmtMessageText->setText(_messageText);
    _fmtMessageText->setAcceptDrops(false);
    _fmtMessageText->setReadOnly(true);
    _fmtMessageText->moveCursor(QTextCursor::Start);
    _fmtMessageText->setFont(_st.fonttext);
    _fmtMessageText->move(_st.radius * 2, _st.fontname->height + _st.radius * 4);
    _fmtMessageText->show();
    _deleteBtn = std::make_unique<FlatButton>(this, "Delete", _st.button);
    resize(width(), expectedHeight());
    // connect(reactionChoseBox.get(), SIGNAL(currentIndexChanged(int)), SLOT(reactionChange(int)));
    _deleteBtn->setClickCallback([&]() { onDelete(); });
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
    QString datetime     = dateTimeMessage.toString("d никогда hh:mm");
    auto    datetimeRect = QRect(margin * 3 + usernameRect.right(), margin * 2 + 1, _st.fontdate->width(datetime), _st.fontdate->height);
    p.drawText(datetimeRect, datetime, Style::al_center);

    QWidget::paintEvent(e);
}

void MessageWidget::resizeEvent(QResizeEvent* e)
{
    geometryChanged(e->size().height() - e->oldSize().height());
    if (_messageFlags & MessageFlag::Deleted) return QWidget::resizeEvent(e);
    int deleteButtonX = width() - _st.radius - _deleteBtn->width();
    _deleteBtn->move(deleteButtonX, _st.radius);
    _fmtMessageText->resize(width() - _st.radius * 4 - 1, _fmtMessageText->document()->size().height());

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
}

void MessageWidget::reactionChange(int index)
{
    if (reactionUserOnMessage != reactions::NON)
    {
        --reactionMap[reactionUserOnMessage];
        for (auto countReaction : reactionMap)
        {
            if (countReaction.second <= 0)
            {
                reactionMapLabelIcon->find(countReaction.first).value()->clear();
                reactionMapLabel->find(countReaction.first).value()->clear();
            }
        }
    }

    auto reactionSelection{[&](int reactionNumber) {
        reactionMapLabelIcon->find(--reactionNumber)
            .value()
            ->setPixmap(
                pixmapIcon->find(reactionNumber)
                    .value()[0]
                    .scaled(QSize(Style::valueDPIScale(16), Style::valueDPIScale(16)), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        reactionMapLabel->find(reactionNumber).value()->setText(QString::number(++reactionMap[reactionNumber]));
        reactionUserOnMessage = static_cast<reactions>(reactionNumber);
    }};
    switch (index)
    {
        case 1:
        case 2:
        case 3:
        case 4:
        {
            reactionSelection(index);
        }
        break;
        default:
        {
            reactionUserOnMessage = reactions::NON;
        }
        break;
    }
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
    dateTimeMessage.setSecsSinceEpoch(utc);
    update();
}

void MessageWidget::setReactionMap(std::map<int, int> newReactionMap)
{
    reactionMap = std::move(newReactionMap);
    update();
}

void MessageWidget::clearMessage()
{
    _fmtMessageText->hide();
    _deleteBtn->hide();
}

int MessageWidget::expectedHeight()
{
    if (!_fmtMessageText) return 0;
    if (_messageFlags & MessageFlag::Deleted)
    {
        return _st.fontname->height + _st.radius * 2;
    }
    return _st.radius * 6 + _st.fontname->height + _fmtMessageText->document()->size().height();
}