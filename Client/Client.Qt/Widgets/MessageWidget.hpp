#ifndef QMESSAGEWIDGET_H
#define QMESSAGEWIDGET_H
constexpr auto EMPTY_MESSAGE      = "Empty message";
constexpr auto EMPTY_USER_NAME    = "You";

#include "pch.hpp"

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
     * @param new text message as string of String.
     */
    void setStdMessageText(std::string newMessage);
    /**
     * @brief Method for method for changing the nickname of the message sender.
     * @param new nickname as string of QString.
     */
    void setUserName(QString newUserName);
    /**
     * @brief Method for method for changing the nickname of the message sender.
     * @param new nickname as string of String.
     */
    void setStdUserName(std::string newUserName);
    /**
     * @brief constructor for displaying a message from a user on the screen.
     * @param text of message as string of QStrings.
     * @param nickname as string of QStrings.
     * @param object of the QListWidgetItem type.
     * @param object of the QWidget type.
     */
    MessageWidget(QString textMessage, QString nameOfUser, QListWidgetItem* Item,
                  QWidget* parent = nullptr);
    /**
     * @brief constructor for displaying a message from a user on the screen.
     * @param text of message as string of QStrings.
     * @param object of the QListWidgetItem type.
     */
    MessageWidget(QString textMessage, QListWidgetItem* Item);
    /**
     * @brief constructor for displaying a message from a user on the screen.
     * @param object of the QListWidgetItem type.
     */
    explicit MessageWidget(QListWidgetItem* Item);
    /**
     * @brief destructor for clearing memory.
     */
    ~MessageWidget();

private slots:
    void deleteButtonClick();
    void reactionChange(int index);

private:
    QListWidgetItem* messageItem;
    // Layouts
    QVBoxLayout* mainLayout;
    QHBoxLayout* UpLevelLayout;
    QHBoxLayout* DownLevelLayout;
    // Message
    FlatPlainTextEdit* messageTextEdit;
    // UpLevelLayout
    Label* userNameLabel;
    Label* reactionLabel;
    Label* reactionLabelIcon;
    QSpacerItem* horizontalUpLeftSpacer;
    QSpacerItem* horizontalUpRightSpacer;
    DateTimeEdit* messageDateTimeEdit;
    QPixmap *LikeIcon;
    // DownLevelLayout
    ComboBox* reactionChoseBox;
    FlatButton* deleteButton;
    QSpacerItem* horizontalDownSpacer;
    // delMessage - shows that the message has been deleted
    Label* delMessage;

    QString messageText;
    QString userName;
    QDateTime dateTimeMessage;
    QString reactionOnMessage;
    QStringList itemReactionList;
    std::map<std::string, int> reactionMap{{"Like", 0}};
    bool messageDel;
    void initializationUi();
    bool isReaction(QString reaction);
    void updateWidget();
    void uiConnect();
    void clearMessage();
};

#endif  // QMESSAGEWIDGET_H
