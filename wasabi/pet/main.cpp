#include "widget.h"

#include <QApplication>
#include <QMainWindow>
#include <QWindow>
#include <QSharedMemory>


int main(int argc, char *argv[])
{



    QApplication a(argc, argv);

    Widget* w = new Widget();

    w->show();

    //w->lower();

    w->setWindowIcon(QIcon(":/p/images/icon2.png"));

    return a.exec();

}
