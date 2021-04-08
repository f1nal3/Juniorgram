#ifndef QMESSAGEWIDGET_H
#define QMESSAGEWIDGET_H
constexpr auto NO_SELECTED        = "Choose";
constexpr auto NO_SELECTED_RETURN = "Null";
constexpr auto EMPTY_MESSAGE      = "Empty message";
constexpr auto EMPTY_USER_NAME    = "You";

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QTime>
#include <QTimeEdit>
#include <QWidget>
#include <QtWidgets/QMainWindow>
#include <Widgets/FlatButton.hpp>

class MessageWidget : public QWidget
{
    Q_OBJECT

public:
    void setThisItem(QListWidgetItem* Item);
    void setMessageText(QString newMessage);
    void setUserName(QString newUserName);

    explicit MessageWidget(QString textMessage, QString nameOfUser, QListWidgetItem* Item,
                           QWidget* parent = nullptr);
    explicit MessageWidget(QString textMessage, QListWidgetItem* Item);
    explicit MessageWidget(QListWidgetItem* Item);
    ~MessageWidget();

private slots:
    void deleteButtonClick();
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
