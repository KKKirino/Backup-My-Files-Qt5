#include "mainwindow.h"
#include "homewidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    HomeWidget h;

    w.show();
    return a.exec();
}
