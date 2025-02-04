#include "mainwindow.h"
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet(R"(
        QSlider::groove:horizontal {
            border: 1px solid #565a5e;
            height: 10px;
            background: #eee;
            margin: 0px;
            border-radius: 4px;
        }
        QSlider::handle:horizontal {
            background: black;
            border: 1px solid #565a5e;
            width: 24px;
            height: 8px;
            border-radius: 4px;
        }
    )");
    MainWindow w;
    w.resize(600,400);
    w.show();
    return a.exec();
}
