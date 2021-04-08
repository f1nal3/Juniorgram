#ifndef QMESSAGEWIDGET_H
#define QMESSAGEWIDGET_H
constexpr auto NO_SELECTED        = "Choose";
constexpr auto NO_SELECTED_RETURN = "Null";
constexpr auto EMPTY_MESSAGE      = "Empty message";
constexpr auto EMPTY_USER_NAME    = "You";

#include <QtWidgets/QMainWindow>
#include <QComboBox>
#include <QGridLayout>
#include <QTime>
#include <QTimeEdit>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

/** @class MessageWidget
 *  @brief 
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
     * @param new text message as string of QStrings.
     */
    void setMessageText(QString newMessage);
    /**
     * @brief Method for method for changing the nickname of the message sender.
     * @param new nickname as string of QStrings.
     */
    void setUserName(QString newUserName);

    MessageWidget(QString textMessage, QString nameOfUser, QListWidgetItem* Item,
                           QWidget* parent = nullptr);
    MessageWidget(QString textMessage, QListWidgetItem* Item);
    explicit MessageWidget(QListWidgetItem* Item);
    ~MessageWidget();

private slots:
    /**
     * @brief
     */
    void deleteButtonClick();
    /**
     * @brief 
     * @param 
     */
    void reactionChange(QString newReaction);

private:
    QListWidgetItem* messageItem;
    // Layouts
    QVBoxLayout* mainLayout;
    QHBoxLayout* UpLevelLayout;
    QHBoxLayout* DownLevelLayout;
    // Message
    QTextEdit* messageTextEdit;
    // UpLevelLayout
    QLabel* userNameLabel;
    QLabel* reactionLabel;
    QSpacerItem* horizontalUpLeftSpacer;
    QSpacerItem* horizontalUpRightSpacer;
    QTimeEdit* messageTimeEdit;
    // DownLevelLayout
    QComboBox* reactionChoseBox;
    QPushButton* deleteButton;
    QSpacerItem* horizontalDownSpacer;
    // Del
    QLabel* delMessage;

    QString messageText;
    QString userName;
    QTime timeMessage;
    QString reactionOnMessage;
    std::map<std::string, int> reactionMap{{"Like", 0}};
    bool messageDel;
    void initializationUi();
    bool isReaction();
    QString getReaction();
    void updateWidget();
    void uiConnet();
    void clearMessage();
};

#endif  // QMESSAGEWIDGET_H
