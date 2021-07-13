#pragma once
constexpr auto EMPTY_MESSAGE   = "Empty message";
constexpr auto EMPTY_USER_NAME = "You";
constexpr auto COUNT_REACTION  = 4;

#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <ctime>

#include "Widgets/ComboBox.hpp"
#include "Widgets/DateTimeEdit.hpp"
#include "Widgets/FlatButton.hpp"
#include "Widgets/InputFields.hpp"
#include "Widgets/Label.hpp"
#include "Widgets/TimeEdit.hpp"

class ChatHistory;

/** @class MessageWidget
 *  @brief message visual representation in chat
 */
class MessageWidget : public QWidget
{
    Q_OBJECT

public:
    enum class MessageFlag
    {
        Uploading = 1u << 0u,
        Edited    = 1u << 1u,
        Deleted   = 1u << 2u,
    };

    typedef QFlags<MessageFlag> MessageFlags;

    /**
     * @brief constructor for displaying a message from a user on the screen.
     * @param history parent widget
     * @param message Message from user.
     * @param utc Seconds since epoch.
     * @param username User nickname.
     */
    MessageWidget(QWidget* history, QString  message, qint64 utc, QString  username,
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
    void setReactionMap(std::map<int, int> newReactionMap);

    void setIndex(int left, int index, int right)
    {
        _left  = left;
        _index = index;
        _right = right;

        update();
    }

    int expectedHeight();
public slots:
    void onDelete();

signals:
    void geometryChanged(int);

protected:
    void paintEvent(QPaintEvent* e) override;
    void resizeEvent(QResizeEvent* e) override;

private slots:
    void reactionChange(int index);

private:
    void clearMessage();

private:
    enum reactions
    {
        LIKE,
        DISLIKE,
        FIRE,
        CAT,
        NON
    };

    // Message
    std::unique_ptr<FlatTextEdit> _fmtMessageText;
    std::unique_ptr<FlatButton>        _deleteBtn;

    QMap<int, Label*>* reactionMapLabel;
    QMap<int, Label*>* reactionMapLabelIcon;

    QMap<int, QPixmap*>* pixmapIcon;

    // DownLevelLayout
    std::unique_ptr<ComboBox> reactionChoseBox;

    int                         _left = 0, _index = 0, _right = 0;
    QString                     _messageText;
    QString                     _username;
    QDateTime                   dateTimeMessage;
    reactions                   reactionUserOnMessage;
    QStringList                 itemReactionList;
    MessageFlags                _messageFlags;
    const Style::MessageWidget& _st;

    std::map<int, int> reactionMap{{reactions::LIKE, 0}, {reactions::DISLIKE, 0}, {reactions::FIRE, 0}, {reactions::CAT, 0}};
};
