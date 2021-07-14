#include <QDebug>

#include "Application.hpp"
#include "Style/StyleBasic.hpp"

int main(int argc, char* argv[])
{
    Application* qapp = new Application(argc, argv);
    qapp->create();
    qapp->show();
    qDebug() << sizeof(int);
    qDebug() << sizeof(Style::font);
    qDebug() << sizeof(Style::FlatButton);
    qDebug() << sizeof(Style::FlatTextEdit);
    qDebug() << sizeof(Style::MessageWidget);
    return qapp->exec();
}
