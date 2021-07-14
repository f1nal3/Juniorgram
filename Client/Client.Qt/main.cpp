#include <QDebug>

#include "Application.hpp"
#include "Style/StyleBasic.hpp"

int main(int argc, char* argv[])
{
    Application* qapp = new Application(argc, argv);
    qapp->create();
    qapp->show();
    return qapp->exec();
}
