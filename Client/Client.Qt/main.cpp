#include "mainWindow.h"
#include "login.h"

#include <QtWidgets/qapplication.h>
#include <QtCore/qdebug.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login login;
    login.show();
    return a.exec();
}
