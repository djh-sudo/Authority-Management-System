#include "modify.h"
#include "ui_modify.h"
#include<vector>
#include<QString>
#include<string>
#include"User.h"
#include"Object.h"
#include"mainwindow.h"
#include<QLineEdit>
#include<iostream>
#include<QDebug>
#include"name.h"
#include<QMessageBox>
#include"gui.h"
using namespace std;
extern vector<User>usergroup;
extern vector<Object>objectgroup;
string user;
string pass;
int index=0;//判断修改密码是否符合规则
Modify::Modify(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Modify)
{
    ui->setupUi(this);
    pro=new MainWindow();
    pro->Base_Start();
    ui->lineEdit->setStyleSheet("border:3px solid gray;border-radius:12px;padding:1px 4px");
    ui->lineEdit_2->setStyleSheet("border:3px solid gray;border-radius:12px;padding:1px 4px");
    ui->lineEdit_3->setStyleSheet("border:3px solid gray;border-radius:12px;padding:1px 4px");
    ui->lineEdit_4->setStyleSheet("border:3px solid gray;border-radius:12px;padding:1px 4px");
    QImage*img=new QImage;
    img->load(":/pass.png");
    ui->label_7->setPixmap(QPixmap::fromImage(*img));
}

Modify::~Modify()
{
    delete ui;
}

void Modify::on_lineEdit_textChanged(const QString &arg1)
{
    for(int i=0;i<usergroup.size();i++){
        if(usergroup[i].Get_Username()==ui->lineEdit->text().toStdString()){
            ui->lineEdit->setStyleSheet("QLineEdit{border:3px solid rgb(0,238,0);border-radius:12px;padding:1px 4px}");
            return;
        }
        else{
            ui->lineEdit->setStyleSheet("border:3px solid gray;border-radius:12px;padding:1px 4px");
        }
    }
    return;
}

void Modify::on_lineEdit_4_textChanged(const QString &arg1)
{
    if(arg1!=""&&arg1==ui->lineEdit_3->text()){
        ui->lineEdit_4->setStyleSheet("QLineEdit{border:3px solid rgb(0,238,0);border-radius:12px;padding:1px 4px}");
        index=1;
    }
    else{
        index=0;
        ui->lineEdit_4->setStyleSheet("border:3px solid rgb(255,0,0);border-radius:12px;padding:1px 4px");
    }
}

void Modify::on_pushButton_2_clicked()//提交修改
{
    user=ui->lineEdit->text().toStdString();
    pass=ui->lineEdit_2->text().toStdString();
    for(int i=0;i<usergroup.size();i++){
        if(user==usergroup[i].Get_Username()&&md(pass)==usergroup[i].Get_password()&&index){
            {
                pass=ui->lineEdit_4->text().toStdString();
                usergroup[i].Modify_password(md(pass));
                pro->Writeout_User("user.txt");
                QMessageBox::information(this,"information","修改成功,请记住你的密码!");
                gui =new GUI();//跳转至登录界面
                gui->setWindowTitle("User Authentication System @copy right by DJH");
                gui->setFixedSize(600,350);
                gui->setAttribute(Qt::WA_DeleteOnClose);
                gui->show();
                this->close();
            }
            return;
        }
    }
    ui->lineEdit_2->clear();
    return;
}

void Modify::on_lineEdit_3_textChanged(const QString &arg1)//检查密码是否符合要求
{
    if(arg1!=ui->lineEdit_4->text()){
        ui->lineEdit_4->setStyleSheet("border:3px solid rgb(255,0,0);border-radius:12px;padding:1px 4px");
        index=0;
    }
    else{
        ui->lineEdit_4->setStyleSheet("border:3px solid rgb(0,238,0);border-radius:12px;padding:1px 4px");
        index=1;
    }
    if(arg1.length()>=6){
        QByteArray ch = arg1.toLatin1();
        char *s = ch.data();
        char *p = ch.data();
        while (*s && *s >= '0' && *s <= '9')
            s++;//如果是数字，指针后移
        while ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z'))
            p++;//如果是字母，指针后移
        //判断密码是否为字母数字组合
        if (*s == NULL)
        {//纯数字
            ui->label_6->setText("密码不能为纯数字");
            index=0;
            return;
        }
        else if (*p == NULL)
        {//纯字母
            ui->label_6->setText("密码不能为纯字母");
            index=0;
            return;
        }
        else{
            ui->label_6->clear();
            index=1;
        }
    }
    else
    {
        ui->label_6->setText("密码不能少于6位");
        return;
    }
}

void Modify::on_toolButton_pressed()//按键显示明文
{
    ui->lineEdit_4->setEchoMode(QLineEdit::Normal);
}
void Modify::on_toolButton_released()//释放显示密文
{
    ui->lineEdit_4->setEchoMode(QLineEdit::Password);
}
void Modify::on_pushButton_3_clicked()//返回登录界面
{
    gui =new GUI();
    gui->setWindowTitle("User Authentication System @copy right by DJH");
    gui->setFixedSize(600,350);
    gui->setAttribute(Qt::WA_DeleteOnClose);
    gui->show();
    this->close();
}
