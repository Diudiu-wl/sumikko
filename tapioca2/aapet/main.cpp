#include "widget.h"

#include <QApplication>
#include <QMainWindow>
#include <QWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget* w = new Widget();

    w->show();    
    //w->lower();

    w->setWindowIcon(QIcon(":/p/im/icon2.png"));

    return a.exec();
}
