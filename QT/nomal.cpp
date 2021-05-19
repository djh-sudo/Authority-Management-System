#include "nomal.h"
#include "ui_nomal.h"
#include<QTableWidget>
#include<vector>
#include"User.h"
#include"Object.h"
#include<QString>
#include<map>
#include<QIcon>
using namespace std;
extern vector<User>usergroup;
extern vector<Object>objectgroup;
extern int groupid;
Nomal::Nomal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Nomal)
{
    ui->setupUi(this);
    int id=groupid;
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"Object"<<"Read"<<"Update"<<"Insert"<<"Delete");
    int count=objectgroup.size();
    ui->tableWidget->setRowCount(count);
    ui->lineEdit->setText(QString::fromStdString(usergroup[id].Get_Username()));
    ui->lineEdit_2->setText(QString::number(usergroup[id].Get_id()));
    ui->tableWidget->setIconSize(QSize(30,30));
    QIcon l1;
    QIcon l2;
    QIcon l3;
    l1.addFile(":/0.png",QSize(40,40));
    l2.addFile(":/1.png",QSize(40,40));
    l3.addFile(":/2.png",QSize(40,40));
    for(int k=0;k<objectgroup.size();k++){
        ui->tableWidget->setItem(k,0,new QTableWidgetItem(QString::fromStdString(objectgroup[k].Get_Filename())));
        if(usergroup[id].OBJ[k].Get_Read()){
            if(usergroup[id].OBJ[k].Get_Delegate_Read())
                ui->tableWidget->setItem(k,1,new QTableWidgetItem(l3, "+"));
            else
                ui->tableWidget->setItem(k,1,new QTableWidgetItem(l2, "-"));
        }
        else
            ui->tableWidget->setItem(k,1,new QTableWidgetItem(l1, "*"));
        if(usergroup[id].OBJ[k].Get_Update()){
            if(usergroup[id].OBJ[k].Get_Delegate_Update())
                ui->tableWidget->setItem(k,2,new QTableWidgetItem(l3,"+"));
            else
                ui->tableWidget->setItem(k,2,new QTableWidgetItem(l2, "-"));
        }else
            ui->tableWidget->setItem(k,2,new QTableWidgetItem(l1, "*"));

        if(usergroup[id].OBJ[k].Get_Insert()){
            if(usergroup[id].OBJ[k].Get_Delegate_Insert())
                ui->tableWidget->setItem(k,3,new QTableWidgetItem(l3, "+"));
            else
                ui->tableWidget->setItem(k,3,new QTableWidgetItem(l2, "-"));
        }else
            ui->tableWidget->setItem(k,3,new QTableWidgetItem(l1, "*"));
        if(usergroup[id].OBJ[k].Get_Delete()){
            if(usergroup[id].OBJ[k].Get_Delegate_Delete())
                ui->tableWidget->setItem(k,4,new QTableWidgetItem(l3, "+"));
            else
                ui->tableWidget->setItem(k,4,new QTableWidgetItem(l2, "-"));
        }else
            ui->tableWidget->setItem(k,4,new QTableWidgetItem(l1, "*"));
    }
}

Nomal::~Nomal()
{
    delete ui;
}
