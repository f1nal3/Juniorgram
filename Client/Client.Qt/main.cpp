#include "mainwindow.h"
#include "login.h"

#include <QtWidgets/qapplication.h>
#include <QtCore/qdebug.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    login loginWindow;
    loginWindow.show();
    return a.exec();
}
