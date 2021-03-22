#include "app.h"
#include "pch.h"

int main(int argc, char* argv[])
{
    QApplication qapp(argc, argv);
    App::create();
    App::show();
    return qapp.exec();
}
