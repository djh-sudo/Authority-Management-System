#include "mainwindow.h"
#include <QApplication>
#include<dialog.h>
#include<gui.h>
#include<admin.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog enterwindow;
    enterwindow.show();
    enterwindow.setWindowTitle("User Authentication System @copy right by DJH");
    enterwindow.setFixedSize(600,350);
    if(enterwindow.exec() == QDialog::Accepted){
        MainWindow w;
        w.setFixedSize(1136,660);
        w.setWindowTitle("User Authentication System-[Command Line] @copy right by DJH");
        w.show();
        return a.exec();
    }
    else{
        GUI logwindow;
        logwindow.setWindowTitle("User Authentication System-[log] @copy right by DJH");
        logwindow.setFixedSize(560,320);
        logwindow.show();
        return a.exec();
    }
}
