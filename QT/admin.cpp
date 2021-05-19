#include "admin.h"
#include "ui_admin.h"
#include"mainwindow.h"
#include<vector>
#include<string>
#include<QListWidget>
#include<QTableWidget>
#include<QMessageBox>
#include<QColor>
#include<QList>
#include<QString>
#include<QDebug>
#include<QLabel>
#include"name.h"
#include<QDebug>
#include<fstream>
#include<algorithm>
using namespace std;
extern vector<User>usergroup;
extern vector<Object>objectgroup;
extern map<int,int>r;
int id=0;
vector<int>ro;
vector<int>co;
vector<int>state;//改变前权限变化情况
Admin::Admin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Admin)
{
    ui->setupUi(this);
    state.clear();
    ui->pushButton->setDefault(false);
    QList<QString>  fileNames;
    fileNames.clear();
    for(int i=0;i<usergroup.size();i++){
        fileNames<<":/p"+QString::number(i+1)+".png";
    }
    ui->listWidget->setIconSize(QSize(50,50));//
    int i=0;
    for(int k=0;k<fileNames.size();k++)//逐次提取文件名称
    {
        QString str="user_name: "+QString::fromStdString(usergroup[i].Get_Username())+"\nuser_id :"+QString::number(usergroup[i].Get_id());
        ui->listWidget->addItem(new QListWidgetItem(QIcon(fileNames[k%11]),str));
        i++;
    }
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"Object"<<"Read"<<"Update"<<"Insert"<<"Delete");
    int count=objectgroup.size();
    ui->tableWidget->setRowCount(count);
}

Admin::~Admin()
{
    delete ui;
}
string charactor(int s){
    if(s==1)
        return"read";
    if(s==2)
        return "upd";
    if(s==3)
        return "ins";
    if(s==4)
        return "del";
}
void Admin::on_listWidget_clicked(const QModelIndex &index)
{
    if(ro.size()!=0){
        if(QMessageBox::Yes==QMessageBox::question(this,"提示","修改内容还未保存!是否放弃修改",QMessageBox::Yes|QMessageBox::No,QMessageBox::No))
        {
            ro.clear();
            co.clear();
            state.clear();
        }
        else
            return;
    }
    id=index.row();
    ui->lineEdit->setText(QString::fromStdString(usergroup[id].Get_Username()));
    ui->lineEdit_2->setText(QString::number(usergroup[id].Get_id()));
    ui->lineEdit_3->setText(QString::fromStdString(md(usergroup[id].Get_Username())));
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
                ui->tableWidget->setItem(k,1,new QTableWidgetItem(l3, QString::fromStdString("["+usergroup[r[usergroup[id].OBJ[k].Get_Source_Read()]].Get_Username()+"]+")));
            else
                ui->tableWidget->setItem(k,1,new QTableWidgetItem(l2, QString::fromStdString("["+usergroup[r[usergroup[id].OBJ[k].Get_Source_Read()]].Get_Username()+"]-")));
        }
        else
            ui->tableWidget->setItem(k,1,new QTableWidgetItem(l1, "*"));
        if(usergroup[index.row()].OBJ[k].Get_Update()){
            if(usergroup[index.row()].OBJ[k].Get_Delegate_Update())
                ui->tableWidget->setItem(k,2,new QTableWidgetItem(l3,QString::fromStdString("["+usergroup[r[usergroup[id].OBJ[k].Get_Source_Update()]].Get_Username()+"]+")));
            else
                ui->tableWidget->setItem(k,2,new QTableWidgetItem(l2, QString::fromStdString("["+usergroup[r[usergroup[id].OBJ[k].Get_Source_Update()]].Get_Username()+"]-")));
        }else
            ui->tableWidget->setItem(k,2,new QTableWidgetItem(l1, "*"));

        if(usergroup[id].OBJ[k].Get_Insert()){
            if(usergroup[id].OBJ[k].Get_Delegate_Insert())
                ui->tableWidget->setItem(k,3,new QTableWidgetItem(l3, QString::fromStdString("["+usergroup[r[usergroup[id].OBJ[k].Get_Source_Insert()]].Get_Username()+"]+")));
            else
                ui->tableWidget->setItem(k,3,new QTableWidgetItem(l2, QString::fromStdString("["+usergroup[r[usergroup[id].OBJ[k].Get_Source_Insert()]].Get_Username()+"]-")));
        }else
            ui->tableWidget->setItem(k,3,new QTableWidgetItem(l1, "*"));
        if(usergroup[id].OBJ[k].Get_Delete()){
            if(usergroup[id].OBJ[k].Get_Delegate_Delete())
                ui->tableWidget->setItem(k,4,new QTableWidgetItem(l3, QString::fromStdString("["+usergroup[r[usergroup[id].OBJ[k].Get_Source_Delete()]].Get_Username()+"]+")));
            else
                ui->tableWidget->setItem(k,4,new QTableWidgetItem(l2, QString::fromStdString("["+usergroup[r[usergroup[id].OBJ[k].Get_Source_Delete()]].Get_Username()+"]-")));
        }else
            ui->tableWidget->setItem(k,4,new QTableWidgetItem(l1, "*"));
    }
}
string Change(int s){
    if(s==7)
        return"+";
    if(s==3)
        return"-";
    if(s==1)
        return "*";
}
void Modify(int k,int uid,int oid,bool auth,bool CA=false){
    if(k==1){
        if(auth){
            usergroup[0].DelegateRead(objectgroup[oid],usergroup[uid],CA);
            return;
        }else{
            usergroup[0].EraseRead(usergroup[uid],objectgroup[oid]);
            return;
        }
    }
    if(k==2){
        if(auth){
            usergroup[0].DelegateUpdate(objectgroup[oid],usergroup[uid],CA);
            return;
        }else{
            usergroup[0].EraseUpdate(usergroup[uid],objectgroup[oid]);
            return;
        }
    }
    if(k==3){
        if(auth){
            usergroup[0].DelegateInsert(objectgroup[oid],usergroup[uid],CA);
            return;
        }else{
            usergroup[0].EraseInsert(usergroup[uid],objectgroup[oid]);
            return;
        }
    }
    if(k==4){
        if(auth){
            usergroup[0].DelegateDelete(objectgroup[oid],usergroup[uid],CA);
            return;
        }else{
            usergroup[0].EraseDelete(usergroup[uid],objectgroup[oid]);
            return;
        }
    }
}
void Admin::on_pushButton_clicked()//第一个元素为obj的下标，第二个为权限类型
{
    QIcon l1;
    QIcon l2;
    QIcon l3;
    l1.addFile(":/0.png",QSize(40,40));
    l2.addFile(":/1.png",QSize(40,40));
    l3.addFile(":/2.png",QSize(40,40));
    ui->textEdit->clear();
    if(QMessageBox::Yes==QMessageBox::question(this,"提示","是否提交修改内容?",QMessageBox::Yes|QMessageBox::No,QMessageBox::No))
    {
        if(id==0){
            QMessageBox::warning(this,"waning","\t\t非法操作![请勿试图修改root权限]\n");
            return;
        }
        string s=usergroup[0].Get_Username();
        ofstream out(md(s), ios::app);
        for(int k=0,j=0;k<ro.size();k++,j++){
            string now=ui->tableWidget->item(ro[k],co[j])->text().toStdString();
            string past=Change(state[k]);
            if(past!=now){//前后不一致
                if(now[0]=='+'){
                    ui->textEdit->append(QString::fromStdString("grant auth -"+charactor(co[j])+" of -"+objectgroup[ro[k]].Get_Filename()+" to -"+usergroup[id].Get_Username()+" with option"));
                    ui->tableWidget->setItem(ro[k],co[j],new QTableWidgetItem(l3,"+"));
                    out<<endl<<"grant auth -"+charactor(co[j])+" of -"+objectgroup[ro[k]].Get_Filename()+" to -"+usergroup[id].Get_Username()+" with option";
                    Modify(co[j],id,ro[k],true,true);
                    continue;
                }
                if(now[0]=='-'){
                    ui->textEdit->append(QString::fromStdString("grant auth -"+charactor(co[j])+" of -"+objectgroup[ro[k]].Get_Filename()+" to -"+usergroup[id].Get_Username()));
                    ui->tableWidget->setItem(ro[k],co[j],new QTableWidgetItem(l2,"-"));
                    out<<endl<<"grant auth -"+charactor(co[j])+" of -"+objectgroup[ro[k]].Get_Filename()+" to -"+usergroup[id].Get_Username();
                    Modify(co[j],id,ro[k],true);
                    continue;
                }
                if(now[0]=='*'){
                    ui->textEdit->append(QString::fromStdString("revoke auth -"+charactor(co[j])+" of -"+objectgroup[ro[k]].Get_Filename()+" from -"+usergroup[id].Get_Username()));
                    ui->tableWidget->setItem(ro[k],co[j],new QTableWidgetItem(l1,"*"));
                    out<<endl<<"revoke auth -"+charactor(co[j])+" of -"+objectgroup[ro[k]].Get_Filename()+" from -"+usergroup[id].Get_Username();
                    Modify(co[j],id,ro[k],false);
                    continue;
                }
            }
            else
                continue;
        }
        state.clear();
        out.close();
        ro.clear();
        co.clear();
        return;
    }
    else return;
}
void Admin::on_tableWidget_itemClicked(QTableWidgetItem *item)//单击触发事件
{
    int row=item->row();
    int column=item->column();
    string temp=ui->tableWidget->item(row,column)->text().toStdString();
    if(ui->tableWidget->item(row,column)->text()!=""){
        ro.push_back(row);
        co.push_back(column);
        if(temp[temp.length()-1]=='+'){
            state.push_back(7);
            return;
        }
        if(temp[temp.length()-1]=='-'){
            state.push_back(3);
            return;
        }

        if(temp[temp.length()-1]=='*'){
            state.push_back(1);
            return;
        }
    }
    return;
}

void Admin::on_pushButton_2_clicked()//重置权限
{
    for(int i=0;i<usergroup.size();i++){
        string path=md(usergroup[i].Get_Username());
        ofstream out(path);
        if (out.is_open())
        {
            out.close();
        }
        else{
            QMessageBox::warning(this,"warning","part of files error[not exist]");
            return;
        }
    }
    QMessageBox::warning(this,"information","重置成功![已恢复出厂设置,重新启动程序有效]");
}
void Admin::on_pushButton_3_clicked()
{

    for(int i=0;i<usergroup.size();i++){
        string path=md(usergroup[i].Get_Username());
        ofstream out(path);
        if (out.is_open())
        {
            out.close();
        }
        else{
            QMessageBox::warning(this,"warning","part of files error[not exist]");
            return;
        }
        out.close();
    }
    for(int i=0;i<usergroup.size();i++){
        string path=md(usergroup[i].Get_Username());
        ofstream out(path);
        for(int k=0;k<objectgroup.size();k++){
            if(!usergroup[i].OBJ[k].Delegate_Read_Pool.empty()){
                for(set<int>::iterator it=usergroup[i].OBJ[k].Delegate_Read_Pool.begin();it!=usergroup[i].OBJ[k].Delegate_Read_Pool.end();it++){
                    if(usergroup[r[*it]].OBJ[k].Get_Delegate_Read()){
                    out<<"grant auth -read of -"+objectgroup[k].Get_Filename()+" to -"+usergroup[r[*it]].Get_Username()+" with option"<<endl;
                    }
                    else
                    {
                      out<<"grant auth -read of -"+objectgroup[k].Get_Filename()+" to -"+usergroup[r[*it]].Get_Username()<<endl;
                    }
                }
            }

            if(!usergroup[i].OBJ[k].Delegate_Insert_Pool.empty()){
                for(set<int>::iterator it=usergroup[i].OBJ[k].Delegate_Insert_Pool.begin();it!=usergroup[i].OBJ[k].Delegate_Insert_Pool.end();it++){
                    if(usergroup[r[*it]].OBJ[k].Get_Delegate_Insert()){
                    out<<"grant auth -ins of -"+objectgroup[k].Get_Filename()+" to -"+usergroup[r[*it]].Get_Username()+" with option"<<endl;
                    }
                    else
                    {
                      out<<"grant auth -ins of -"+objectgroup[k].Get_Filename()+" to -"+usergroup[r[*it]].Get_Username()<<endl;
                    }
                }
            }

            if(!usergroup[i].OBJ[k].Delegate_Update_Pool.empty()){
                for(set<int>::iterator it=usergroup[i].OBJ[k].Delegate_Update_Pool.begin();it!=usergroup[i].OBJ[k].Delegate_Update_Pool.end();it++){
                    if(usergroup[r[*it]].OBJ[k].Get_Delegate_Update()){
                    out<<"grant auth -upd of -"+objectgroup[k].Get_Filename()+" to -"+usergroup[r[*it]].Get_Username()+" with option"<<endl;
                    }
                    else
                    {
                      out<<"grant auth -upd of -"+objectgroup[k].Get_Filename()+" to -"+usergroup[r[*it]].Get_Username()<<endl;
                    }
                }
            }

            if(!usergroup[i].OBJ[k].Delegate_Delete_Pool.empty()){
                for(set<int>::iterator it=usergroup[i].OBJ[k].Delegate_Delete_Pool.begin();it!=usergroup[i].OBJ[k].Delegate_Delete_Pool.end();it++){
                    if(usergroup[r[*it]].OBJ[k].Get_Delegate_Delete()){
                    out<<"grant auth -del of -"+objectgroup[k].Get_Filename()+" to -"+usergroup[r[*it]].Get_Username()+" with option"<<endl;
                    }
                    else
                    {
                      out<<"grant auth -del of -"+objectgroup[k].Get_Filename()+" to -"+usergroup[r[*it]].Get_Username()<<endl;
                    }
                }
            }
        }
        out.close();
    }
    QMessageBox::information(this,"information","\t\t文件更新完毕");
}
