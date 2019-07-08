#include "main_window.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setFixedSize(120, 50);
    w.show();

    w.move(QApplication::desktop()->width() - 150, QApplication::desktop()->height() - 100);

    return a.exec();
}
