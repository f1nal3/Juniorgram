#include "App.hpp"
#include "pch.hpp"

int main(int argc, char* argv[])
{
    QApplication qapp(argc, argv);
    App::create();
    App::show();
    return qapp.exec();
}
