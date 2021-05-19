#include "dialog.h"
#include "ui_dialog.h"
#include<QPixmap>
#include<QDebug>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QImage*img=new QImage;
    QImage*img1=new QImage;
    QImage*img2=new QImage;
    img->load(":/start1.png");
    img1->load(":/cmd.png");
    img2->load(":/gui.png");
    ui->label->setPixmap(QPixmap::fromImage(*img));
    ui->radioButton_2->setChecked(true);
    ui->label_3->setPixmap(QPixmap::fromImage(*img1));

    connect(ui->radioButton_2,&QRadioButton::clicked,this,[=]{
        ui->label_3->clear();
        ui->label_3->setPixmap(QPixmap::fromImage(*img1));
    });

    connect(ui->radioButton,&QRadioButton::clicked,this,[=]{
        ui->label_3->clear();
        ui->label_3->setPixmap(QPixmap::fromImage(*img2));
    });
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    if(ui->radioButton_2->isChecked()){
        accept();
        return;
    }
    if(ui->radioButton->isChecked()){
        reject();
        return;
    }else return;
}
void Dialog::closeEvent(QCloseEvent *event) //系统自带退出确定程序
{
    exit(1);
}
