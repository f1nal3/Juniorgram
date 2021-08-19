#pragma once
#include <QDateTime>
#include <memory>

#include "Widgets/Buttons.hpp"
#include "Widgets/InputFields.hpp"
#include "Widgets/ReactionLayout.hpp"
#include "Widgets/ReplyWidget.hpp"

class ChatHistory;

/** @class MessageWidget
 *  @brief message visual representation in _chat
 */
class MessageWidget : public QWidget
{
    Q_OBJECT

public:
    /// Message flag
    enum class MessageFlag
    {
        Uploading = 1u << 0u,
        Edited    = 1u << 1u,
        Deleted   = 1u << 2u,
    };

    /// Message flags
    typedef QFlags<MessageFlag> MessageFlags;

    /**
     * @brief constructor for displaying a message from a user on the screen.
     * @param history parent widget
     * @param message Message from user.
     * @param userId of user.
     * @param messageId of message.
     * @param utc Seconds since epoch.
     * @param username User nickname.
     */
    MessageWidget(QWidget* history, QString message, uint64_t userId, uint64_t messageId, qint64 utc, QString username,
                  const Style::MessageWidget& st = st::defaultMessageWidget);

    /**
     * @brief Method for method for changing the message.
     * @param new text message as string of QString.
     */
    void setMessageText(const QString& newMessage);
    /**
     * @brief Set new nickname if user have changed it.
     * @param newUserName new nickname for user.
     */
    void setUserName(const QString& newUserName);
    /**
     * @brief Method for changing the data and time of the message sender.
     * @param utc Seconds since epoch.
     */
    void setTime(qint64 utc);
    /**
     * @brief Method for changing reaction map.
     * @param new reactions of map with kay int and value int.
     */
    void setReactionMap(const std::map<uint32_t, uint32_t>& newReactionMap);

    /// Set message index in array
    void setIndex(int index)
    {
        _index = index;
        update();
    }

    /// Possible height of message widget
    int expectedHeight() const;

    /// Message DB
    bool isTheMessage(uint64_t messageId, uint64_t userId) const { return messageId == _messageId && userId == _userId; }

public slots:
    void onDelete();
    void createReply();

signals:
    void geometryChanged(int);
    void createReplySignal(ReplyWidget*);

protected:
    void paintEvent(QPaintEvent* e) override;
    void resizeEvent(QResizeEvent* e) override;

private:
    void clearMessage();

private:
    std::unique_ptr<FlatTextEdit>   _fmtMessageText;
    std::unique_ptr<FlatButton>     _menuBtn;
    std::unique_ptr<FlatButton>     _reactionsBtn;
    std::unique_ptr<ReactionLayout> _reactions;
    std::unique_ptr<ReactionLayout> _reactionsInMenu;
    int32_t                         _index = 0;

    uint64_t                    _userId;
    uint64_t                    _messageId;
    QString                     _messageText;
    QString                     _username;
    QDateTime                   _datetime;
    MessageFlags                _messageFlags;
    const Style::MessageWidget& _st;


    std::unique_ptr<FlatButton>   _replyBtn;
    ReplyWidget*                  _replyWidget = nullptr;
};
