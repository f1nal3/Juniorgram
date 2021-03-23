#pragma once

#include <QWidget>
#include <QStringListModel>
#include <QPushButton>
#include <QLineEdit>
#include <QListView>
#include <QGridLayout>
#include <textedit.h>

/*QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE*/

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void updateChat();
    void updateMessagesList_User();
    void updateMessagesList_Bot();
private:
    //Ui::MainWindow *ui;
    QPushButton* sendButton;
    QPushButton* randomButton;
    TextEdit* textEdit;
    //QLineEdit* lineEdit;
    QListView* chatView;
    QStringListModel *model;
    QStringList *messagesList;
};
