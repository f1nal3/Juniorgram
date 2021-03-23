#pragma once

#include <QWidget>
#include <QStringListModel>
#include <QPushButton>
#include <QLineEdit>
#include <QListView>
#include <QGridLayout>
#include <textEdit.h>

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
    QPushButton* sendButton;
    QPushButton* randomButton;
    TextEdit* textEdit;
    QListView* chatView;
    QStringListModel *model;
    QStringList *messagesList;
};
