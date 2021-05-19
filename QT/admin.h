#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>
#include"User.h"
#include"Object.h"
#include<QTableWidget>
#include"mainwindow.h"
namespace Ui {
class Admin;
}

class Admin : public QDialog
{
    Q_OBJECT

public:
    explicit Admin(QWidget *parent = nullptr);
    ~Admin();

private slots:
    void on_listWidget_clicked(const QModelIndex &index);
    void on_pushButton_clicked();
    void on_tableWidget_itemClicked(QTableWidgetItem *item);
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::Admin *ui;
};

#endif // ADMIN_H
