#ifndef QMESSAGEWIDGET_H
#define QMESSAGEWIDGET_H
constexpr auto EMPTY_MESSAGE      = "Empty message";
constexpr auto EMPTY_USER_NAME = "You";
constexpr auto COUNT_REACTION = 4;

#include "pch.hpp"

#include <ctime>

#include "Widgets/FlatButton.hpp"
#include "Widgets/InputFields.hpp"
#include "Widgets/Label.hpp"
#include "Widgets/TimeEdit.hpp"
#include "Widgets/DateTimeEdit.hpp"
#include "Widgets/ComboBox.hpp"

/** @class MessageWidget
 *  @brief message visual representation in chat
 */
class MessageWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Method for changing the object of the QListWidgetItem class in the MessageWidget.
     * @param object reference of class QListWidgetItem.
     */
    void setThisItem(QListWidgetItem* Item);
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
     * @param text of message as string of QStrings.
     * @param nickname as string of QStrings.
     * @param object of the QListWidgetItem type.
     * @param boolean indicator of a deleted message.
     * @param object of the QWidget type.
     */
    MessageWidget(QString textMessage, QString nameOfUser, QListWidgetItem* Item,
                  bool deletedMessage, QWidget* parent = nullptr);
    /**
     * @brief constructor for displaying a message from a user on the screen.
     * @param text of message as string of std::strings.
     * @param nickname as string of std::strings.
     * @param object of the QListWidgetItem type.
     * @param boolean indicator of a deleted message.
     */
    MessageWidget(std::string textMessage, std::string nameOfUser, QListWidgetItem* Item,
                  bool deletedMessage);
    /**
     * @brief constructor for displaying a message from a user on the screen.
     * @param text of message as string of QStrings.
     * @param object of the QListWidgetItem type.
     * @param boolean indicator of a deleted message.
     */
    MessageWidget(QString textMessage, QListWidgetItem* Item, bool deletedMessage);
    /**
     * @brief constructor for displaying a message from a user on the screen.
     * @param object of the QListWidgetItem type.
     * @param boolean indicator of a deleted message.
     */
    explicit MessageWidget(QListWidgetItem* Item, bool deletedMessage);
    /**
     * @brief destructor for clearing memory.
     */
    ~MessageWidget();

private slots:
    void deleteButtonClick();
    void reactionChange(int index);

private:
    enum reactions
    {
        LIKE,
        DISLIKE,
        FIRE,
        CAT,
        NON
    };
    QListWidgetItem* messageItem;
    // Layouts
    QVBoxLayout* mainLayout;
    QHBoxLayout* UpLevelLayout;
    QHBoxLayout* DownLevelLayout;
    // Message
    FlatPlainTextEdit* messageTextEdit;
    // UpLevelLayout
    Label* userNameLabel;

    QMap<int, Label*>* reactionMapLabel;

    QMap<int, Label*>* reactionMapLabelIcon;

    Label* reactionLabelIconLike;
    Label* reactionLabelIconDislike;
    Label* reactionLabelIconFire;
    Label* reactionLabelIconCat;

    QSpacerItem* horizontalUpLeftSpacer;
    QSpacerItem* horizontalUpRightSpacer;
    DateTimeEdit* messageDateTimeEdit;

    QMap<int, QPixmap*> *pixmapIcon;

    // DownLevelLayout
    ComboBox* reactionChoseBox;
    FlatButton* deleteButton;
    QSpacerItem* horizontalDownSpacer;
    // delMessage - shows that the message has been deleted
    Label* delMessage;

    bool messageDeleted;
    QString messageText;
    QString userName;
    QDateTime dateTimeMessage;
    reactions reactionUserOnMessage;
    QStringList itemReactionList;
    std::map<int, int> reactionMap{
        {reactions::LIKE, 0},
        {reactions::DISLIKE, 0},
        {reactions::FIRE, 0},
        {reactions::CAT, 0}
    };
    void initializationUiNotDelete();
    void initializationUiDelete();
    void updateWidget();
    void uiConnect();
    void clearMessage();
};

#endif  // QMESSAGEWIDGET_H
