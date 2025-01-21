#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Задание глобального стиля
    a.setStyleSheet("QWidget { font-size: 20px; }");
    MainWindow w;
    w.show();
    return a.exec();
}
