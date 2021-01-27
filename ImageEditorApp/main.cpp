#include <QApplication>
//#include <QMainWindow>
#include "mainwindow.h"
#include <iostream>
#include <QDebug>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("Image Editor");
    QString windowTitle = window.windowTitle();
    window.show();
    // or use showFullScreen
    // or show maximized
    return app.exec();
}
