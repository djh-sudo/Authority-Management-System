#include "gui.h"
#include "ui_gui.h"
#include<vector>
#include"User.h"
#include"Object.h"
#include"mainwindow.h"
#include"md5.h"
#include"name.h"
#include<string>
#include<QString>
#include<QMessageBox>
#include<QDebug>
#include<nomal.h>
#include"modify.h"
using namespace std;
extern vector<User>usergroup;
extern vector<Object>objectgroup;
extern map<int, int>r;
int groupid=0;
GUI::GUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GUI)
{
    ui->setupUi(this);
    QImage*img=new QImage;
    img->load(":/log.png");
    ui->label_4->setPixmap(QPixmap::fromImage(*img));
    ui->lineEdit_3->setFocus();
    ui->lineEdit_2->setStyleSheet("border:3px solid gray;border-radius:12px;padding:1px 4px");
    ui->lineEdit_3->setStyleSheet("border:3px solid gray;border-radius:12px;padding:1px 4px");
    pro=new MainWindow();
    pro->Base_Start();
}

GUI::~GUI()
{
    delete ui;
}

void GUI::on_pushButton_clicked()
{
    QString username=ui->lineEdit_3->text();
    QString password=ui->lineEdit_2->text();
    for(int i=0;i<usergroup.size();i++){
        if(usergroup[i].Get_Username()==username.toStdString()&&usergroup[i].Get_password()==md(password.toStdString())){
            QMessageBox::information(this,"welcome "+QString::number(usergroup[i].Get_id()),"\t欢迎 "+QString::fromStdString(usergroup[i].Get_Username())+" 使用用户授权系统");
            if(i==0){
                apro=new Admin();
                apro->setWindowTitle("Admin Authentication System[GUI] @copy right 2020 by DJH");
                apro->setFixedSize(1055,580);
                apro->setAttribute(Qt::WA_DeleteOnClose);
                apro->show();
                GUI::close();
                return;
            }
            else{
                groupid=i;
                apro1=new Nomal();
                apro1->setWindowTitle("User Authentication System[GUI] @copy right 2020 by DJH");
                apro1->setFixedSize(730,400);
                apro1->setAttribute(Qt::WA_DeleteOnClose);
                apro1->show();
                GUI::close();
                return;
            }
        }
    }
    QMessageBox::warning(this,"提示","\t用户名或者密码不正确!\n或者未注册!");//
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    return;
}

void GUI::on_pushButton_2_clicked()//退出操作
{
    if(QMessageBox::Yes==QMessageBox::question(this,"提示","确认退出登录环节？",QMessageBox::Yes|QMessageBox::No,QMessageBox::No))
    {
        exit(1);
    }
    else
        return;
}

void GUI::on_toolButton_clicked()//修改密码操作
{
    mod=new Modify();
    mod->setWindowTitle("Modify the password @copy right 2020 by DJH");
    mod->setAttribute(Qt::WA_DeleteOnClose);
    mod->show();
    GUI::close();
}
