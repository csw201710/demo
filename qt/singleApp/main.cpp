#include "mainwindow.h"
#include <QApplication>
#include "qtsingleapplication/qtsingleapplication.h"
#include <QMessageBox>
int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    QtSingleApplication a(argc,argv);
    if(a.isRunning()){QMessageBox::information(0,"title","has running!");a.sendMessage("raise_window_noop"); return 0;}


    MainWindow w;

    a.setActivationWindow(&w);
    QObject::connect(&a,&QtSingleApplication::messageReceived,&a,&QtSingleApplication::activateWindow);

    w.show();

    return a.exec();
}
