#ifndef MODIFY_H
#define MODIFY_H

#include <QDialog>
#include"mainwindow.h"
#include"gui.h"
#include"admin.h"
namespace Ui {
class Modify;
}
class GUI;
class Modify : public QDialog
{
    Q_OBJECT

public:
    explicit Modify(QWidget *parent = nullptr);
    MainWindow *pro;
    GUI*gui;
    ~Modify();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);
    void on_lineEdit_4_textChanged(const QString &arg1);
    void on_pushButton_2_clicked();
    void on_lineEdit_3_textChanged(const QString &arg1);
    void on_toolButton_pressed();
    void on_toolButton_released();
    void on_pushButton_3_clicked();

private:
    Ui::Modify *ui;
};

#endif // MODIFY_H
