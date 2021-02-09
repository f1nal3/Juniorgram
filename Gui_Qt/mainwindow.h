#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <Serialize.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void updateChat();
    void updateMessagesList_User();
    void updateMessagesList_Bot();
private:
    Ui::MainWindow *ui;
    QStringListModel *model;
    QStringList *messagesList;
    Serialize *serialize;
};
#endif // MAINWINDOW_H
