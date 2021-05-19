#include"mainwindow.h"
#include"ui_mainwindow.h"
#include<QTextCursor>
#include<QTextDocument>
#include<QString>
#include<iostream>
#include<QTextBlock>
#include<QDebug>
#include<vector>
#include<string>
#include<windows.h>
#include<cstdlib>
#include<fstream>
#include<QFile>
#include<QMessageBox>
#include<QTextEdit>
#include<map>
#include<set>
#include"name.h"
#include"admin.h"
#include"Object.h"
#include"User.h"
#include"Method.h"
#include"nomal.h"
#include<conio.h>
#include<cstdlib>
using namespace std;

vector<User>usergroup;//用户数组
vector<Object>objectgroup;//对象数组
map<int, int>r;//id和数组下标映射关系
vector<string>cmd;//指令集合
vector<string>cmd_temp;//存储进程运行的所有命令集合
string output="";//暂存输出字符串
int flag = 0;//区分系统登录和人工命令的差异
extern int groupid;//记录登录用户的id
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);//初始化
    username="##@Guest~>>>";//默认为游客权限
    id=-1;//未登录状态的id小于0
    comset.clear();//清空指令集合
    Base_Start();//从磁盘中加载至内存
    ui->textEdit->setAcceptRichText(false);//textedit设置为非富文本[html]形式
    currentPath=QCoreApplication::applicationDirPath();//获取完整当前路径
    QStringList list =currentPath.split('/');//切片
    currentPath=list[list.length()-1];//取最后一个分片，只显示最后一个路径
    midlength=currentPath.length()+username.length()+2;//整个前缀长度由路径和用户名组成
    Inicialize(1);//初始化界面
    this->pro=new QProcess;//设置此程序的进程
    pro->setProgram("DJH");//设置进程
    //处理光标和相应
    connect(ui->textEdit,SIGNAL(cursorPositionChanged()),this,SLOT(Set_Cursor()));//构建光标的信号和槽
    connect(pro,SIGNAL(readyRead()),this,SLOT(Read_Output()));
    //设置样式
    Text_Type="SimSun";//宋体
    size=13;//字体大小
    SetFont();//字体样式设置
    BackGroud();//背景和字体颜色设置
}
MainWindow::~MainWindow()//析构函数
{
    delete ui;
}
void MainWindow::SetFont()//字体样式设置函数
{
    QFont font(Text_Type,size,70);//设置字体样式
    ui->textEdit->setFont(font);//设置textedit
}
void MainWindow::BackGroud(){//背景及文字颜色初始化
    QPalette pal = ui->textEdit->palette();
    QColor color(0,0,0);//黑色
    pal.setColor(QPalette::Base,color);//背景
    QColor color1(149,196,102);//绿色
    pal.setColor(QPalette::Text,color1);//字体
    ui->textEdit->setPalette(pal);
    ui->textEdit->repaint();
    //需要重新渲染文字，对整个屏幕生效
    QString temp = ui->textEdit->toPlainText();
    ui->textEdit->setText(temp);
}
void MainWindow::on_textEdit_textChanged()//用户光标处理[textchange信号]
{
    QTextCursor cursor=ui->textEdit->textCursor();//获取用户光标
    cursor.movePosition(QTextCursor::End);//光标移动到最后一行
    QTextDocument *doc=ui->textEdit->document();//得到文本框的文字
    int Line_Number=cursor.blockNumber();//行号
    QTextBlock block=doc->findBlockByLineNumber(Line_Number);//选取最后一行的文字
    QString selectLine=block.text();//获取最后一行的字符串
    if(selectLine.mid(2+currentPath.length(),13)=="the password:")//输入密码模式，需要隐藏明文
    {
        ui->textEdit->setTextColor(QColor(0,0,0));
    }
    else if(selectLine.back()==">"||selectLine.back()==":")//其余模式字体颜色
    {
        ui->textEdit->setTextColor(QColor(238,233,120));
    }
    lastCommand=selectLine;//将最后一行字符串存到LastCommand中
}
void MainWindow::Set_Cursor()//设置光标到最后一行
{
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);//光标移动到最后一行

}
void MainWindow::Read_Output()//读取textedit的内容
{
    out=pro->readAll();
    if(out!=""){
        ui->textEdit->append(QString::fromLocal8Bit(out));
    }
}
void MainWindow::Inicialize(int flag)//清屏[clc]+初始化
{
    ui->textEdit->setTextColor(QColor(149,196,102));
    ui->textEdit->setText(welcome);
    if(flag == 1)//初始化
    {
        ui->textEdit->append("["+currentPath+"]##@Guest~>>>");
    }
    ui->textEdit->setFocus();//重设光标
    ui->textEdit->installEventFilter(this);//设置完后自动调用其EventFilter函数
    Set_Cursor();//光标移动到最后
    lastCommand = "["+currentPath+"]##@Guest~>>>";
}
bool MainWindow::eventFilter(QObject *target, QEvent *event){//事件过滤器,改写键盘事件[虚函数,多态性]
    static int count=0;
    if(target == ui->textEdit)
    {
        if(event->type() == QEvent::KeyPress)//用户按键
        {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);
            if(k->modifiers()== Qt::ControlModifier && k->key()==Qt::Key_A){//不响应全选键Ctrl+A
                return true;//终止流程
            }
            if(k->key() == Qt::Key_Return||k->key() == Qt::Key_Enter)//字母键盘回车|数字键盘回车
            {
                Handle();//回车表示提交信息，交给Handle()函数处理
                return true;
            }
            if(k->key() == Qt::Key_Backspace)//用户不得删除前面的信息
                if(lastCommand.length() == midlength)//最后一行字符串长度=前缀长度时，不再响应回车
                    return true;
            if(k->key()==Qt::Key_Up)//上键
            {
                QTextCursor cursor = ui->textEdit->textCursor();
                cursor.movePosition(QTextCursor::End);
                while(lastCommand.length()!=midlength){//先删除已有的命令
                    cursor.deletePreviousChar();
                }
                count++;
                if(comset.size()-count>=0&&comset.size()-count<comset.size()){//边界判断
                    ui->textEdit->insertPlainText(comset[comset.size()-count]);//输出上一条命令
                    ui->textEdit->moveCursor(QTextCursor::End);
                }else
                {
                    if(comset.size()-count==0){
                        ui->textEdit->insertPlainText(comset[0]);
                        ui->textEdit->moveCursor(QTextCursor::End);
                    }
                    else {
                        if(count>0)
                            count--;
                        return true;
                    }
                }
            }
            if(k->key()==Qt::Key_Down)//下键
            {
                if(count>0)
                    count--;
                QTextCursor cursor = ui->textEdit->textCursor();
                cursor.movePosition(QTextCursor::End);
                while(lastCommand.length()!=midlength){
                    cursor.deletePreviousChar();
                }
                if(comset.size()-count<comset.size()&&comset.size()-count>=0){//边界判断
                    ui->textEdit->insertPlainText(comset[comset.size()-count]);
                    ui->textEdit->moveCursor(QTextCursor::End);
                }
                else
                {
                    if(comset.size()-count==comset.size()-1){
                        ui->textEdit->insertPlainText(comset[comset.size()-1]);
                        ui->textEdit->moveCursor(QTextCursor::End);
                    }
                    else{
                        count++;
                        return true;
                    }
                }
            }
        }
    }
    return QWidget::eventFilter(target,event);//其余按键可以通过
}
void MainWindow::Handle()//事件处理函数
{
    com=lastCommand.mid(midlength);
    if(com!="")//当前命令
        comset.push_back(com);
    MainProcess(com.toStdString());//处理命令的部分
    com.clear();//清空，便于下一条指令获取
}
void MainWindow::MainProcess(string s)//命令处理函数
{
    Command(s,username,id);//命令译码解析函数
    midlength=currentPath.length()+username.length()+2;
    ui->textEdit->setTextColor(QColor(149,196,102));//处理路径颜色
    ui->textEdit->insertPlainText("\n["+currentPath+"]");
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->setTextColor(QColor(247,155,102));//处理username颜色
    ui->textEdit->insertPlainText(QString::fromStdString(username));
    ui->textEdit->setTextColor(QColor(238,233,120));//处理命令颜色
    if(id==-3)//处理密码颜色
    {
        ui->textEdit->setTextColor(QColor(0,0,0));
    }
    return;
}
void MainWindow::Inicial_Root(User& user) //Root权限初始化
{
    for (int i = 0; i < user.OBJ.size(); i++) {
        user.OBJ[i].Modify_Delegate_Delete(true);
        user.OBJ[i].Modify_Delegate_Insert(true);
        user.OBJ[i].Modify_Delegate_Read(true);
        user.OBJ[i].Modify_Delegate_Update(true);
        user.OBJ[i].Modify_Delete(true);
        user.OBJ[i].Modify_Insert(true);
        user.OBJ[i].Modify_Read(true);
        user.OBJ[i].Modify_Update(true);
        user.OBJ[i].Modify_Source_Read(0);
        user.OBJ[i].Modify_Source_Insert(0);
        user.OBJ[i].Modify_Source_Update(0);
        user.OBJ[i].Modify_Source_Delete(0);
    }
}
void MainWindow::Inicial_Relation()//初始化user_id和数组下标的对应关系
{
    for (int i = 0; i < usergroup.size(); i++) {
        r.insert(map<int, int>::value_type(usergroup[i].Get_id(), i));
    }
}
void MainWindow::Inicial_Command()//基础命令
{
    cmd.push_back("help");
    cmd.push_back("log");
    cmd.push_back("exit");
    cmd.push_back("clc");
    cmd.push_back("out");
}
string MainWindow::Information(int k)//模糊匹配提示信息
{
    switch (k)
    {
    case 0: return "Do you mean 'help'?[this command will give you information]";
    case 1:	return "Do you mean 'log'?[log on the system]";
    case 2: return "Do you mean 'exit'?[exit from the system]";
    case 3: return "Do you mean 'clc'?[clear all]";
    case 4: return "Do you mean 'show object'?[show all the object]";
    case 5: return "Do you mean 'show auth'?[what authentication you have]";
    default:return "No command,you can use the 'help'";
    }
}
void MainWindow::Help()//[help]基础执行
{
    ui->textEdit->append("welcome use the help,if you have any question");
    ui->textEdit->append("please send email to djh113@126.com");
    ui->textEdit->append("[help]  ## give you all command and information");
    ui->textEdit->append("[log]  ## log on this system with username and password");
    ui->textEdit->append("[exit]  ## exit from the system");
    ui->textEdit->append("[clc]  ## clear the screen,clear all thing");
    ui->textEdit->append("[out]  ##logout the system");
}
void MainWindow::Help(int k)//传入参数为用户的id[help]命令执行
{
    Help();
    if (k == -1) return;
    ui->textEdit->append("[auth -X]  ##  show authentication you have about object X");
    ui->textEdit->append("[show obj]  ## show all object in the system");
    ui->textEdit->append("[grant auth -X of -O to -U]  ##  grant the X auth of Object O to User U\n The X should be read/upd/del/ins");
    ui->textEdit->append("[revoke auth -X of -O from -U]  ##  revoke the X auth of Oject O from User U\n The X should be read/upd/del/ins");
    ui->textEdit->append("[chain -X of -O]  ##  show the X auth chain of obj O\nThe X should be read/upd/del/ins");
    if (k == 0) {
        ui->textEdit->append("[show auth of user -U]  ##  show the auth of the user U");
        ui->textEdit->append("[show auth of obj -O]  ##  show the object O with all users");
        ui->textEdit->append("[revoke all -X of -O]  ##  revoke X auth of object O from all users\nThe X should be read/upd/del/ins");
    }
}
void MainWindow::trim(string& s)//字符串去掉空格
{
    int index = 0;
    if (!s.empty())
        while ((index = s.find(' ', index)) != string::npos)
            s.erase(index, 1);
}
void MainWindow::Clear() {//[clc]命令执行
    Inicialize(0);
}
void MainWindow::Exit(int id)//退出系统
{
    if(id!=-1){
        if(QMessageBox::Yes==QMessageBox::question(this,"提示","\t你还未退出登录[out],关闭命令行可能丢失信息，是否关闭命令行?",QMessageBox::Yes|QMessageBox::No,QMessageBox::No))
            exit(0);
    }
    else
        exit(0);
}
int MainWindow::User_Find(string username)//根据用户名返回用户的id[模糊查询返回其反码]
{
    for (int i = 0; i < usergroup.size(); i++) {
        string s;
        s.clear();
        s = usergroup[i].Get_Username();
        transform(s.begin(), s.end(), s.begin(), ::tolower);//大小写不敏感
        if (s.find(username) != string::npos||username.find(s)!=string::npos)//模糊匹配
        {
            if (s == username) {
                return usergroup[i].Get_id();
            }
            else
                return ~usergroup[i].Get_id();
        }
    }
    return -999;
}
int MainWindow::Object_Find(string obj)//查找对象
{
    for (int i = 0; i < objectgroup.size(); i++) {
        string s;
        s.clear();
        s = objectgroup[i].Get_Filename();
        transform(s.begin(), s.end(), s.begin(), ::tolower);//大小写不敏感
        if (s.find(obj) != string::npos||obj.find(s)!=string::npos)//模糊匹配
        {
            if (s == obj) {
                return i;
            }
            else
                return ~i;
        }
    }
    return -99999;
}
void MainWindow::Login(string& name, int&id)//用户登录函数
{
    if(id==-1){
        name="the username:";
        id=-2;
        return;
    }
    else
    {
        ui->textEdit->append("you have loged on this system");
        return;
    }
}
void MainWindow::Login(string& name, int& id,string username,string password)//登录验证函数
{
    if (id == -1)//id==-1表示未登录状态
    {
        for (int i = 0; i < usergroup.size(); i++) {
            if (usergroup[i].Get_Username() == username)//找到用户名
            {
                if (password == usergroup[i].Get_password())//匹配密码
                {
                    name = "##@" + usergroup[i].Get_Username() + "~>>>";
                    id = usergroup[i].Get_id();
                    cmd.push_back("auth-");
                    cmd.push_back("showobj");
                    cmd.push_back("grantauth-of-to-");
                    cmd.push_back("revokeauth-of-from-");
                    cmd.push_back("grantauth-of-to-withoption");
                    if (id == 0)//root拥有更多权限
                    {
                        cmd.push_back("showauthofuser-");
                        cmd.push_back("showauthofobj-");
                        cmd.push_back("chain-of-");
                        cmd.push_back("revokeall-of-");
                    }
                    ui->textEdit->append("log on this system successfully");
                    return;
                }
                ui->textEdit->append("error with the username or password");
                name = "##@Guest~>>>";
                return;
            }
        }
        name = "##@Guest~>>>";
        ui->textEdit->append("error with the username or password");
        return;
    }
    ui->textEdit->append("you have log on the system");
}
void MainWindow::Logout(string& name, int& id)//退出登录系统
{
    if (id != -1) {
        name = "##@Guest~>>>";
        while (cmd.size() != 5)
            cmd.pop_back();
        Clear();
        ui->textEdit->append("you have log out the system");
        Base_End(md(usergroup[r[id]].Get_Username()), cmd_temp);
        id = -1;
        ui->pushButton->setEnabled(true);
    }
    else{
        ui->textEdit->append("you have not log on this system");
    }
}
string MainWindow::Read(bool pre) {
    if (pre)
        return "read";
    else
        return "";
}
string MainWindow::Insert(bool pre) {
    if (pre)
        return "ins";
    else
        return "";
}
string MainWindow::Update(bool pre) {
    if (pre)
        return "upda";
    else
        return "";
}
string MainWindow::Delete(bool pre) {
    if (pre)
        return "dele";
    else
        return"";
}
void MainWindow::Auth(int id, string x) {//根用户的id查看授权信息,仅供查询自己
    //根据Object的名字查找obj的下标
    ui->textEdit->append("");
    for (int i = 0; i < objectgroup.size(); i++) {
        string s = objectgroup[i].Get_Filename();//大小写不敏感
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        if (x == s) {
            if (usergroup[r[id]].OBJ[i].Get_Read()){
                ui->textEdit->insertPlainText(QString::fromStdString(Read(usergroup[r[id]].OBJ[i].Get_Read())+charactor(usergroup[r[id]].OBJ[i].Get_Delegate_Read()))+" ");
                ui->textEdit->moveCursor(QTextCursor::End);
            }
            //                cout << Read(usergroup[r[id]].OBJ[i].Get_Read()) << charactor(usergroup[r[id]].OBJ[i].Get_Delegate_Read()) << " ";
            if (usergroup[r[id]].OBJ[i].Get_Insert()){
                ui->textEdit->insertPlainText(QString::fromStdString(Insert(usergroup[r[id]].OBJ[i].Get_Insert())+charactor(usergroup[r[id]].OBJ[i].Get_Delegate_Insert()))+" ");
                ui->textEdit->moveCursor(QTextCursor::End);
            }
            //                cout << Insert(usergroup[r[id]].OBJ[i].Get_Insert()) << charactor(usergroup[r[id]].OBJ[i].Get_Delegate_Insert()) << " ";
            if (usergroup[r[id]].OBJ[i].Get_Update()){
                ui->textEdit->insertPlainText(QString::fromStdString(Update(usergroup[r[id]].OBJ[i].Get_Update())+charactor(usergroup[r[id]].OBJ[i].Get_Delegate_Update()))+" ");
                ui->textEdit->moveCursor(QTextCursor::End);
            }
            //                cout << Update(usergroup[r[id]].OBJ[i].Get_Update()) << charactor(usergroup[r[id]].OBJ[i].Get_Delegate_Update()) << " ";
            if (usergroup[r[id]].OBJ[i].Get_Delete()){
                ui->textEdit->insertPlainText(QString::fromStdString(Delete(usergroup[r[id]].OBJ[i].Get_Delete())+charactor(usergroup[r[id]].OBJ[i].Get_Delegate_Delete()))+" ");
                ui->textEdit->moveCursor(QTextCursor::End);
            }
            return;
        }
    }
    ui->textEdit->append("No object named "+QString::fromStdString(x));
    return;
}
void MainWindow::Auth_User(string username) {//root查询用户的授权情况
    int index = User_Find(username);
    if (index == -999) {
        ui->textEdit->append("No user named "+QString::fromStdString(username));
        return;
    }
    if (index < 0) {
        ui->textEdit->append("Do you find user "+QString::fromStdString(usergroup[r[~index]].Get_Username()));
        return;
    }
    else {
        for (int i = 0; i < objectgroup.size(); i++) {
            //            cout << objectgroup[i].Get_Filename() << ":";
            ui->textEdit->append(QString::fromStdString(objectgroup[i].Get_Filename())+":");
            if (usergroup[r[index]].OBJ[i].Get_Read()){
                ui->textEdit->insertPlainText(QString::fromStdString(Read(usergroup[r[index]].OBJ[i].Get_Read()))+"["+QString::fromStdString(usergroup[r[usergroup[r[index]].OBJ[i].Get_Source_Read()]].Get_Username())+"]"+QString::fromStdString(charactor(usergroup[r[index]].OBJ[i].Get_Delegate_Read()))+" ");
                ui->textEdit->moveCursor(QTextCursor::End);
            }
            if (usergroup[r[index]].OBJ[i].Get_Insert()){
                ui->textEdit->insertPlainText(QString::fromStdString(Insert(usergroup[r[index]].OBJ[i].Get_Insert()))+"["+QString::fromStdString(usergroup[r[usergroup[r[index]].OBJ[i].Get_Source_Insert()]].Get_Username())+"]"+QString::fromStdString(charactor(usergroup[r[index]].OBJ[i].Get_Delegate_Insert()))+" ");
                ui->textEdit->moveCursor(QTextCursor::End);
            }
            if (usergroup[r[index]].OBJ[i].Get_Update()){
                ui->textEdit->insertPlainText(QString::fromStdString(Update(usergroup[r[index]].OBJ[i].Get_Update()))+"["+QString::fromStdString(usergroup[r[usergroup[r[index]].OBJ[i].Get_Source_Update()]].Get_Username())+"]"+QString::fromStdString(charactor(usergroup[r[index]].OBJ[i].Get_Delegate_Update()))+" ");
                ui->textEdit->moveCursor(QTextCursor::End);
            }
            if (usergroup[r[index]].OBJ[i].Get_Delete()){
                ui->textEdit->insertPlainText(QString::fromStdString(Delete(usergroup[r[index]].OBJ[i].Get_Delete()))+"["+QString::fromStdString(usergroup[r[usergroup[r[index]].OBJ[i].Get_Source_Delete()]].Get_Username())+"]"+QString::fromStdString(charactor(usergroup[r[index]].OBJ[i].Get_Delegate_Delete())));
                ui->textEdit->moveCursor(QTextCursor::End);
            }
        }
    }
}
string MainWindow::charactor(int k) {
    if (k)
        return "+";
    else
        return "-";
}
void MainWindow::Auth_Object(string obj) {//root查询某个对象的授权情况
    int index = Object_Find(obj);//obj在pool中的下标
    if (index == -99999) {
        ui->textEdit->append("No object named "+QString::fromStdString(obj)+"?");
        return;
    }
    if (index < 0) {
        ui->textEdit->append("Do you find object "+QString::fromStdString(objectgroup[~index].Get_Filename())+"?");
        return;
    }
    else
    {
        ui->textEdit->append("Read: ");
        for (int i = 0; i < usergroup.size(); i++) {
            if (usergroup[i].OBJ[index].Get_Read()){
                ui->textEdit->insertPlainText(QString::fromStdString(usergroup[i].Get_Username())+"["+QString::fromStdString(usergroup[r[usergroup[i].OBJ[index].Get_Source_Read()]].Get_Username())+"]"+QString::fromStdString(charactor(usergroup[i].OBJ[index].Get_Delegate_Read()))+" ");
                ui->textEdit->moveCursor(QTextCursor::End);
            }
        }
        ui->textEdit->insertPlainText("\nInsert:");
        ui->textEdit->moveCursor(QTextCursor::End);
        for (int i = 0; i < usergroup.size(); i++) {
            if (usergroup[i].OBJ[index].Get_Insert()){
                ui->textEdit->insertPlainText(QString::fromStdString(usergroup[i].Get_Username())+"["+QString::fromStdString(usergroup[r[usergroup[i].OBJ[index].Get_Source_Insert()]].Get_Username())+"]"+QString::fromStdString(charactor(usergroup[i].OBJ[index].Get_Delegate_Insert()))+" ");
                ui->textEdit->moveCursor(QTextCursor::End);
            }
        }
        ui->textEdit->insertPlainText("\nUpdate:");
        ui->textEdit->moveCursor(QTextCursor::End);
        for (int i = 0; i < usergroup.size(); i++) {
            if (usergroup[i].OBJ[index].Get_Update()){
                ui->textEdit->insertPlainText(QString::fromStdString(usergroup[i].Get_Username())+"["+QString::fromStdString(usergroup[r[usergroup[i].OBJ[index].Get_Source_Update()]].Get_Username())+"]"+QString::fromStdString(charactor(usergroup[i].OBJ[index].Get_Delegate_Update()))+" ");
                ui->textEdit->moveCursor(QTextCursor::End);
            }
        }
        ui->textEdit->insertPlainText("\nDelete:");
        ui->textEdit->moveCursor(QTextCursor::End);
        for (int i = 0; i < usergroup.size(); i++) {
            if (usergroup[i].OBJ[index].Get_Delete()){
                ui->textEdit->insertPlainText(QString::fromStdString(usergroup[i].Get_Username())+"["+QString::fromStdString(usergroup[r[usergroup[i].OBJ[index].Get_Source_Delete()]].Get_Username())+"]"+QString::fromStdString(charactor(usergroup[i].OBJ[index].Get_Delegate_Delete()))+" ");
                ui->textEdit->moveCursor(QTextCursor::End);
            }
        }
    }
}
void MainWindow::Show_Object()//显示所有对象名称
{
    for (int i = 0; i < objectgroup.size(); i++){
        if(i%4==0)
            ui->textEdit->append("");
        ui->textEdit->insertPlainText(QString::fromStdString(objectgroup[i].Get_Filename())+"["+QString::number(objectgroup[i].Get_Id())+"]  ");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
    return;
}
int MainWindow::Grant(int id, string auth, string user, string obj, bool CA) //授权者的id,被授权用户名，授权对象名[Grant]执行
{
    int uid = User_Find(user);
    if (uid == -999) {//user查询失败
        ui->textEdit->append("No user named "+QString::fromStdString(user));
        return 0;
    }
    if (uid < 0) {//user模糊匹配成功
        ui->textEdit->append( "Do you find user " +QString::fromStdString(usergroup[r[~uid]].Get_Username())+"?");
        return 0;
    }
    int oid = Object_Find(obj);
    if (oid == -99999)//obj查询失败
    {
        ui->textEdit->append("No Object named "+QString::fromStdString(obj));
        return 0;
    }
    if (oid < 0)//obj模糊匹配成功
    {
        ui->textEdit->append("Do you find object "+QString::fromStdString(objectgroup[~oid].Get_Filename())+"?");
        return 0;

    }
    if (auth == "read") {
        int back=usergroup[r[id]].DelegateRead(objectgroup[oid], usergroup[r[uid]], CA);
        ui->textEdit->append(QString::fromStdString(output));
        return back;
    }
    if (auth == "upd") {
        int back=usergroup[r[id]].DelegateUpdate(objectgroup[oid], usergroup[r[uid]], CA);
        ui->textEdit->append(QString::fromStdString(output));
        return back;
    }
    if (auth == "del") {
        int back=usergroup[r[id]].DelegateDelete(objectgroup[oid], usergroup[r[uid]], CA);
        ui->textEdit->append(QString::fromStdString(output));
        return back;
    }
    if (auth == "ins") {
        int back=usergroup[r[id]].DelegateInsert(objectgroup[oid], usergroup[r[uid]], CA);
        ui->textEdit->append(QString::fromStdString(output));
        return back;
    }
    ui->textEdit->append("the premeter X must be one of the[read upd del ins]");
    return 0;
}
int MainWindow::Revoke(string auth, string user, string obj)//权限回收[Rvvoke]执行
{
    int uid = User_Find(user);
    if(uid==0){
        ui->textEdit->append("Can't operator the root!");
        return 0;
    }
    if (uid == -999) {
        ui->textEdit->append("No user named "+QString::fromStdString(user));
        return 0;
    }
    if (uid < 0) {
        ui->textEdit->append("Do you find user "+QString::fromStdString(usergroup[r[~uid]].Get_Username())+"?");
        return 0;
    }
    int oid = Object_Find(obj);
    if (oid == -99999) {
        ui->textEdit->append("No object named "+QString::fromStdString(obj));
        return 0;
    }
    if (oid < 0) {
        ui->textEdit->append("Do you find object "+QString::fromStdString( objectgroup[~oid].Get_Filename())+"?");
        return 0;
    }
    if (auth == "read") {
        return usergroup[r[id]].EraseRead(usergroup[r[uid]], objectgroup[oid]);
    }
    if (auth == "upd") {
        return usergroup[r[id]].EraseUpdate(usergroup[r[uid]], objectgroup[oid]);
    }
    if (auth == "del") {
        return usergroup[r[id]].EraseDelete(usergroup[r[uid]], objectgroup[oid]);
    }
    if (auth == "ins") {
        return usergroup[r[id]].EraseInsert(usergroup[r[uid]], objectgroup[oid]);
    }
    ui->textEdit->append("the premeter X must be one of the[read upd del ins]");
    return 0;
}
void MainWindow::Chain(string user, string obj, string auth)//链式查询[chain]执行
{
    vector<int>path;//路径
    path.clear();
    output="";
    int oid = Object_Find(obj);
    if (oid == -99999)//obj查询失败
    {
        ui->textEdit->append("No object named "+QString::fromStdString(obj));
        return;
    }
    if (oid < 0)//obj模糊匹配成功
    {
        ui->textEdit->append("Do you find object "+QString::fromStdString(objectgroup[~oid].Get_Filename())+"?");
        return;
    }
    if (auth == "read") {
        ui->textEdit->insertPlainText("\n"+QString::fromStdString(objectgroup[oid].Get_Filename())+" read:");
        ui->textEdit->moveCursor(QTextCursor::End);
        Read_Chain(usergroup[0], oid, path);
        ui->textEdit->append(QString::fromStdString(output));
        return;
    }
    if (auth == "del") {
        ui->textEdit->insertPlainText("\n"+QString::fromStdString(objectgroup[oid].Get_Filename())+" delete:");
        ui->textEdit->moveCursor(QTextCursor::End);
        Delete_Chain(usergroup[0], oid, path);
        ui->textEdit->append(QString::fromStdString(output));
        return;
    }
    if (auth == "upd") {
        ui->textEdit->insertPlainText("\n"+QString::fromStdString(objectgroup[oid].Get_Filename())+" update:");
        ui->textEdit->moveCursor(QTextCursor::End);
        Update_Chain(usergroup[0], oid, path);
        ui->textEdit->append("\n"+QString::fromStdString(output));
        return;
    }
    if (auth == "ins") {
        ui->textEdit->insertPlainText("\n"+QString::fromStdString(objectgroup[oid].Get_Filename())+" insert:");
        ui->textEdit->moveCursor(QTextCursor::End);
        Insert_Chain(usergroup[0], oid, path);
        ui->textEdit->append(QString::fromStdString(output));
        return;
    }
    ui->textEdit->append("the premeter X must be one of the[read upd del ins]");
    return;
}
void MainWindow::Revoke_All(string user, string obj, string auth)//回收对象
{
    int oid=Object_Find(obj);
    if (oid == -99999) {
        ui->textEdit->append("No object named "+QString::fromStdString(obj));
        return ;
    }
    if (oid < 0) {
        ui->textEdit->append("Do you find object "+QString::fromStdString( objectgroup[~oid].Get_Filename())+"?");
        return ;
    }
    if(auth=="read"){
        for(set<int>::iterator i=usergroup[0].OBJ[oid].Delegate_Read_Pool.begin();i!=usergroup[0].OBJ[oid].Delegate_Read_Pool.end();i++){
            string u=usergroup[r[*i]].Get_Username();
            Revoke(auth,u,obj);
        }
        ui->textEdit->append("Revoke the object"+QString::fromStdString(obj)+" successfully!");
        return;
    }
    if(auth=="ins"){
        for(set<int>::iterator i=usergroup[0].OBJ[oid].Delegate_Insert_Pool.begin();i!=usergroup[0].OBJ[oid].Delegate_Insert_Pool.end();i++){
            string u=usergroup[r[*i]].Get_Username();
            Revoke(auth,u,obj);
        }
        return;
    }
    if(auth=="upd"){
        for(set<int>::iterator i=usergroup[0].OBJ[oid].Delegate_Update_Pool.begin();i!=usergroup[0].OBJ[oid].Delegate_Update_Pool.end();i++){
            string u=usergroup[r[*i]].Get_Username();
            Revoke(auth,u,obj);
        }
        ui->textEdit->append("Revoke the object"+QString::fromStdString(obj)+" successfully!");
        return;
    }
    if(auth=="del"){
        for(set<int>::iterator i=usergroup[0].OBJ[oid].Delegate_Delete_Pool.begin();i!=usergroup[0].OBJ[oid].Delegate_Delete_Pool.end();i++){
            string u=usergroup[r[*i]].Get_Username();
            Revoke(auth,u,obj);
        }
        ui->textEdit->append("Revoke the object"+QString::fromStdString(obj)+" successfully!");
        return;
    }
}
void MainWindow::Function(int k, string& name, int& id, string auth , string user, string obj)//执行命令
{
    switch (k)
    {
    case 0: {//help
        Help(id); break;
    }
    case 1: {//log
        Login(name, id);
        flag = 1;
        break;
    }
    case 2: {//exit
        Exit(id); break;
    }
    case 3: {//clc
        Clear(); break;
    }
    case 4: {//out
        Logout(name, id); break;
        flag = 0;
    }
    case 5: {//auth-X
        Auth(id, obj); break;
    }
    case 6: {//showobj
        Show_Object(); break;
    }
    case 7: {//grantauth-of-to-
        if (Grant(id, auth, user, obj)&&flag) {
            cmd_temp.push_back("grant auth -" + auth + " of -" + obj + " to -" + user);
            //写入文件
        }
        break;
    }
    case 8: {//revokeauth-of-from-
        if (Revoke(auth, user, obj)&&flag) {
            cmd_temp.push_back("revoke auth -" + auth + " of -" + obj + " from -" + user);
            ui->textEdit->append("Revoke is OK!");
            //写入文件
        }break;
    }
    case 9: {//grant with option
        if (Grant(id, auth, user, obj, true)&&flag) {
            cmd_temp.push_back("grant auth -" + auth + " of -" + obj + " to -" + user + " with option");
        } break;
    }
    case 10: {//showauthofuser-
        Auth_User(user); break;
    }
    case 11: {//showauthofobj-
        Auth_Object(obj); break;
    }
    case 12: {//Chain-of-
        Chain(user, obj, auth); break;
    }
    case 13:{//revoke all -X of -O
        Revoke_All(user,obj,auth);
        cmd_temp.push_back("revoke all -" + auth + " of -" + obj);
        break;
    }
    default:
        break;
    }
}
void MainWindow::Command(string s, string& username, int& id)//命令解析函数[译码]
{
    if(id==-2){
        user_name=s;
        username="the password:";
        id=-3;
        return;
    }
    if(id==-3){
        user_password=s;
        comset.pop_back();
        ui->textEdit->setTextColor(QColor(149,196,102));
        id=-1;
        Login(username,id,user_name,md(user_password));
        return;
    }
    transform(s.begin(), s.end(), s.begin(), ::tolower);//大小写不敏感
    int i = 0;
    for (i; i < cmd.size(); i++) {
        if (i < 5) {//不带参数的基本命令，直接匹配
            if (s.find(cmd[i]) != string::npos || cmd[i].find(s) != string::npos) {
                if (s == cmd[i]) {
                    Function(i, username, id);
                    break;
                }
                else
                {
                    if (s != "") {
                        ui->textEdit->append(QString::fromStdString(Information(i)));
                        break;
                    }
                    break;
                }
            }
        }
        else {//带参数的命令，需要提取参数
            string copy;
            copy = s;
            vector<int>index;
            vector<int>padding;
            index.clear();
            padding.clear();
            for (int k = 0; k < copy.size(); k++)
            {
                if (copy[k] == '-')
                {
                    index.push_back(k);//提取定界符-
                }
                int flag = 90;
                if (!index.empty())
                    flag = index.back();
                if (copy[k] == ' ' && k > flag && index.size() > padding.size()) {//获取-后的第一个空格
                    padding.push_back(k);//提取定界符空格
                }
            }
            //补齐定界符
            while (index.size() < 3) {
                index.push_back(copy.size() - 1);
            }
            while (padding.size() < 3) {
                padding.push_back(copy.size() - 1);
            }
            //提取三个参数 p1 p2 p3
            string pre1 = copy.substr(index[0] + 1, padding[0] - index[0]);//第一个参数
            string pre2 = copy.substr(index[1] + 1, padding[1] - index[1]);//第二个参数
            string pre3 = copy.substr(index[2] + 1, padding[2] - index[2]);//第三个参数
            int pos;
            //去掉空格
            trim(copy);//copy为原始命令
            trim(pre1);
            trim(pre2);
            trim(pre3);
            //去掉copy中的参数
            if ((pos = copy.find(pre1)) != -1) {
                copy.replace(pos, pre1.size(), "");
            }
            if ((pos = copy.find(pre2)) != -1) {
                copy.replace(pos, pre2.size(), "");
            }
            if ((pos = copy.find(pre3)) != -1) {
                copy.replace(pos, pre3.size(), "");
            }
            if (pre2 == "") {
                pre2 = pre1;
                pre3 = pre1;
            }
            if (cmd[i] == copy) //命令严格匹配
            {
                Function(i, username, id, pre1, pre3, pre2);//id是登录者的id
                break;
            }
            else;
        }
    }
    if (i == cmd.size())
        ui->textEdit->append("there is no command with "+QString::fromStdString(s));
    return;
}
void MainWindow::Readin_Object(string path)//读入对象信息
{
    ifstream in(path.data());
    if (!in.is_open()) {
        QMessageBox::information(this,"提示","参数配置有误,请与Admin联系!\n或者尝试重新启动!\nerror_number[obj_in]");
        exit(0);
    }
    Object temp;
    while (in >> temp) {
        objectgroup.push_back(temp);//cin被重载了
    }
    in.close();
    return;
}
void MainWindow::Readin_User(string path)//读入用户信息
{
    ifstream in(path.data());
    if (!in.is_open()) {
        QMessageBox::information(this,"提示","参数配置有误,请与Admin联系!\n或者尝试重新启动!\nerror_number[user_in]");
        exit(0);
    }
    User temp(objectgroup);
    while (in >> temp) {
        usergroup.push_back(temp);
    }
    in.close();
}
void MainWindow::Writeout_User(string path)//用户数据内存写入文件
{
    ofstream out(path);
    if (out.is_open())
    {
        for (int i = 0; i < usergroup.size(); i++)
            out << usergroup[i];
        out.close();
    }
}
void MainWindow::Base_Start()//程序起始初始化
{
    objectgroup.clear();
    usergroup.clear();
    r.clear();
    cmd.clear();
    Readin_Object("object.txt");
    Readin_User("user.txt");
    Inicial_Root(usergroup[0]);
    Inicial_Command();
    Inicial_Relation();
    vector<string>fcmd;
    for (int i = 0; i < usergroup.size(); i++)//完成基本信息读入内存
    {
        string buffer = "";
        fcmd.clear();
        ifstream in(md(usergroup[i].Get_Username()));
        if (!in.is_open()) {
            QMessageBox::information(this,"提示","参数配置有误,请与Admin联系!\n或者尝试重新启动!\nerror_number[md5]");
            exit(0);
        }
        while (!in.eof()) {
            getline(in, buffer);
            fcmd.push_back(buffer);
        }
        in.close();
        int id = -1;
        string username = "##@Guest~>>>";
        Login(username, id, usergroup[i].Get_Username(), usergroup[i].Get_password());
        for (int k = 0; k < fcmd.size(); k++) {
            Command(fcmd[k], username, id);
        }
        Command("out", username, id);
    }
}
void MainWindow::Base_End(string path, vector<string>c)//程序结束时指令存取到txt
{
    ofstream out(path, ios::app);
    if (out.is_open())
    {
        for (int i = 0; i < c.size(); i++)
            out << endl << c[i];
        out.close();
    }
}
void MainWindow::on_pushButton_clicked()//开启GUI按钮触发事件
{
    if(id==0){
        ad =new Admin();
        ad->setWindowTitle("Admin Authentication System[GUI] @copy right 2020 by DJH");
        ad->setFixedSize(1055,580);
        ad->setAttribute(Qt::WA_DeleteOnClose);
        ad->show();
        ui->pushButton->setEnabled(false);
    }
    if(id>0){
        groupid=r[id];
        no =new Nomal();
        no->setWindowTitle("User Authentication System[GUI] @copy right 2020 by DJH");
        no->setFixedSize(730,400);
        no->setAttribute(Qt::WA_DeleteOnClose);
        no->show();
        ui->pushButton->setEnabled(false);
    }
}
