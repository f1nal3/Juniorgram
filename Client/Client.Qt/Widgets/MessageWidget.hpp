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
    /**
     * @brief Method for method for changing the message.
     * @param new text message as string of QString.
     */
    void setMessageText(QString newMessage);
    /**
     * @brief Method for method for changing the message.
     * @param new text message as string of std::string.
     */
    void setStdMessageText(std::string newMessage);
    /**
     * @brief Method for changing the nickname of the message sender.
     * @param new nickname as string of QString.
     */
    void setUserName(QString newUserName);
    /**
     * @brief Method for changing the nickname of the message sender.
     * @param new nickname as string of std::string.
     */
    void setStdUserName(std::string newUserName);
    /**
     * @brief Method for changing the data and time of the message sender.
     * @param new datatime of QDateTime.
     */
    void setDateTime(QDateTime newDataTime);
    /**
     * @brief Method for changing the data and time of the message sender.
     * @param new datatime of std::time_t.
     */
    void setStdTime_tDateTime(std::time_t newDataTime);
    /**
     * @brief Method for changing reaction map.
     * @param new reactions of map with kay int and value int.
     */
    void setReactionMap(std::map<int, int> newReactionMap);
    /**
     * @brief constructor for displaying a message from a user on the screen.
     * @param history parent widget
     * @param message is QString.
     * @param utc seconds since epoch.
     * @param username nickname.
     */
    MessageWidget(QWidget* history, QString message, qint64 utc, QString username);
    /**
     * @brief destructor for clearing memory.
     */
    ~MessageWidget() override;

private slots:
    void deleteButtonClick();
    void reactionChange(int index);

private:
    void initializationUiNotDelete();
    void initializationUiDelete();
    void updateWidget();
    void connectUi();
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
    // Layouts
    std::unique_ptr<QVBoxLayout> mainLayout;
    std::unique_ptr<QHBoxLayout> UpLevelLayout;
    std::unique_ptr<QHBoxLayout> DownLevelLayout;
    // Message
    FlatPlainTextEdit* messageTextEdit;
    // UpLevelLayout
    Label* userNameLabel;

    QMap<int, Label*>* reactionMapLabel;

    QMap<int, Label*>* reactionMapLabelIcon;

    std::unique_ptr<QSpacerItem> horizontalUpLeftSpacer;
    std::unique_ptr<QSpacerItem> horizontalUpRightSpacer;
    DateTimeEdit*                messageDateTimeEdit;

    QMap<int, QPixmap*>* pixmapIcon;

    // DownLevelLayout
    std::unique_ptr<ComboBox>    reactionChoseBox;
    std::unique_ptr<FlatButton>  deleteButton;
    std::unique_ptr<QSpacerItem> horizontalDownSpacer;
    // delMessage - shows that the message has been deleted
    Label* delMessage;

    bool               messageDeleted;
    QString            messageText;
    QString            userName;
    QDateTime          dateTimeMessage;
    reactions          reactionUserOnMessage;
    QStringList        itemReactionList;
    std::map<int, int> reactionMap{
        {reactions::LIKE, 0}, {reactions::DISLIKE, 0}, {reactions::FIRE, 0}, {reactions::CAT, 0}};
};
