#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    for(auto counter = 0; counter < 4; ++counter)
    {
    qDebug() << tr("I am working! [Debug info]");
    }

    return a.exec();
}
