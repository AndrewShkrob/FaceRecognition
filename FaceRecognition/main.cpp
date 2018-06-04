#include "MainWindow.hpp"
#include <QApplication>

#include "Settings.hpp"

int main(int argc, char *argv[])
{
    Settings::getInstance();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
