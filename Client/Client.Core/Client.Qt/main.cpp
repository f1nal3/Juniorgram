#include "mainwindow.h"

#include <QtWidgets/qapplication.h>
#include <QtCore/qdebug.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
