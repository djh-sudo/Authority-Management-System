#ifndef GUI_H
#define GUI_H

#include <QDialog>
#include"modify.h"
#include<mainwindow.h>
#include<admin.h>
#include"nomal.h"

namespace Ui {
class GUI;
}
class Modify;
class GUI : public QDialog
{
    Q_OBJECT

public:
    explicit GUI(QWidget *parent = nullptr);
    MainWindow *pro;
    Admin*apro;
    Nomal*apro1;
    Modify*mod;

    ~GUI();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_toolButton_clicked();

private:
    Ui::GUI *ui;
};

#endif // GUI_H
