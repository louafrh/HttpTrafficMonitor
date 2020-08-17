#include "../include/mainwindow.h"
#include "../include/utils.h"

#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Utils::initAppLogging();
    if(argc > 1)
        w.init(argc, argv);
    w.show();
    return a.exec();
}





