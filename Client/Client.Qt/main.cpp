#include "App.hpp"
#include "Application.hpp"

int main(int argc, char* argv[])
{
    Application* qapp = new Application(argc, argv);
    qapp->create();
    qapp->show();
    qapp->setObjectApplication(qapp);
    //QApplication qapp(argc, argv);
    //App::create();
    //App::show();
    //qapp.exec();
    return qapp->exec();
}
