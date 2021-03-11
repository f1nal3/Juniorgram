#pragma once

#include <QMainWindow>
#include <QStringListModel>

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
    Ui::MainWindow *ui;
    QStringListModel *model;
    QStringList *messagesList;
};
