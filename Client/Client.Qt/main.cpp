#include "App.hpp"
#include "Application.hpp"

int main(int argc, char* argv[])
{
    Application* qa = new Application(argc, argv);
    qa->create();
    qa->show();
    //QApplication qapp(argc, argv);
    //App::create();
    //App::show();
    return qa->exec();//qapp.exec();
}
