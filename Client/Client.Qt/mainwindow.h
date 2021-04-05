#pragma once

#include <QMainWindow>
#include <QStringListModel>
#include <QListWidget>
#include "qmessagewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
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
    Ui::MainWindow* ui;
    QStringListModel *model;
    QStringList* messagesList;
    void newMessage(QString textMessage, QString userNameMessage);
    void newMessage(QString textMessage);
};
