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
    QVBoxLayout* mainLayout;

    QHBoxLayout* UpLevelLayout;
    QTimeEdit* messageTimeEdit;
    QLabel* userNameLabel;
    QLabel* reactionLabel;
    QSpacerItem* horizontalUpLeftSpacer;
    QSpacerItem* horizontalUpRightSpacer;

    QTextEdit* messageTextEdit;

    QHBoxLayout* DownLevelLayout;
    QComboBox* reactionChoseBox;
    QPushButton* deleteButton;
    QSpacerItem* horizontalDownSpacer;


    explicit QMessageWidget(QWidget* parent = nullptr);

private:
    void initializationUi();
};

#endif  // QMESSAGEWIDGET_H