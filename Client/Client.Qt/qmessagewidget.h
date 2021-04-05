#ifndef QMESSAGEWIDGET_H
#define QMESSAGEWIDGET_H
constexpr auto NO_SELECTED        = "Choose";
constexpr auto NO_SELECTED_RETURN = "Null";
constexpr auto EMPTY_MESSAGE      = "Empty message";
constexpr auto EMPTY_USER_NAME    = "You";

#include <QtWidgets/QMainWindow>
#include <QCombobox>
#include <QGridLayout>
#include <QTime>
#include <QTimeEdit>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui
{ class QMessageWidget; }
QT_END_NAMESPACE

class QMessageWidget : public QWidget
{
    Q_OBJECT

public:
    // Layouts
    QVBoxLayout* mainLayout;
    QHBoxLayout* UpLevelLayout;
    QHBoxLayout* DownLevelLayout;
    // message
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



    explicit QMessageWidget();
    explicit QMessageWidget(QString textMessage, QString nameOfUser, QWidget* parent = nullptr);
    explicit QMessageWidget(QString textMessage);
    ~QMessageWidget();


private slots:
    void deleteButtonClick();
    void reactionChange(QString newReaction);

private:
    QString messageText;
    QString userName;
    QTime timeMessage;
    QString reactionOnMessage;
    std::map<std::string, int> reactionMap{{"Like", 0}};
    void initializationUi();
    bool isReaction();
    QString getReaction();
    void updateWidget();
    void uiConnet();
};

#endif  // QMESSAGEWIDGET_H