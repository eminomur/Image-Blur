#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // Following line of code hides full screen button from title bar
    w.setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    w.show();

    return a.exec();
}
