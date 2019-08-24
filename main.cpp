#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
extern "C"{
#include "server.h"
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setup_server();
    MainWindow w;
    w.showMaximized();
    //w.show();
    return a.exec();
}

