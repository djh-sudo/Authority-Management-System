#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QProcess>
#include<string>
#include"User.h"
#include"dialog.h"
#include"admin.h"
#include"nomal.h"
using namespace std;
class Admin;
class Nomal;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Admin*ad;
    Nomal*no;
private slots:
    void Set_Cursor();//设置光标
    void Read_Output();
    void on_textEdit_textChanged();
    void BackGroud();
    void on_pushButton_clicked();

private:
    QString welcome="welcome to the commandline\n@copy right 2020 by djh\n";
    QVector<QString>comset;//存储已输入过的命令集合
    QString com;//当前命令
    QString lastCommand;//最后一行的所有字符串
    QString midedCommand;//去掉目录路径后的当前命令，真实的命令
    QProcess *pro;//当前线程
    string user_name;//用户名
    string user_password;//密码
    QByteArray out;//
    QString currentPath;//当前路径的最后一个
    QString Text_Type;//字体样式
    int size;//字体大小
    string username;//用户角色
    int midlength;//有效命令长度
    int id;//登录用户id
public:
    Ui::MainWindow *ui;
    bool eventFilter(QObject *target, QEvent *event);//事件过滤器,对键盘部分按键进行处理
    void Inicialize(int flag);//界面清空
    void SetFont();//设置界面样式
    void Handle();//事件处理函数
    void MainProcess(string s);//命令接受函数
    void Inicial_Root(User& user);//初始root用户权限
    void Inicial_Relation();//初始化id和usergroup下标的映射关系
    void Inicial_Command();//初始化命令集
    string Information(int k);//输出信息提示函数
    void Help();//帮助函数
    void Help(int k);//重载帮助函数
    void trim(string& s);//去掉字符串空格
    void Clear();//屏幕清空命令clc
    void Exit(int id);//退出命令exit
    int User_Find(string username);//根据用户名查找用户id
    int Object_Find(string obj);//根据对象名查找对象id
    void Login(string& name, int&id);//登录函数
    void Login(string& name, int& id, string username, string password);//登录函数重载
    void Logout(string& name, int& id);//退出登录
    string Read(bool pre);//返回read字符串
    string Insert(bool pre);//返回insert字符串
    string Update(bool pre);//返回update字符串
    string Delete(bool pre);//返回delete字符串
    void Auth(int id, string x);//授权信息查询
    void Auth_User(string username);//root查询某个用户授权信息
    string charactor(int k);//是否可以传递授权符号
    void Auth_Object(string obj);//root查询某个对象的授权情况
    void Show_Object();//显示全部的对象名称[id]
    int Grant(int id, string auth, string user, string obj, bool CA = false);//授权函数
    int Revoke(string auth, string user, string obj);//回收授权函数
    void Revoke_All(string user, string obj, string auth);//回收某个对象的某个权限[针对于所有用户]
    void Chain(string user, string obj, string auth);//查询授权链
    void Function(int k, string& name, int& id, string auth = "", string user = "", string obj = "");//命令执行函数
    void Command(string s,string& username,int&id);//命令译码函数
    void Readin_Object(string path);//文件写入内存对象信息
    void Readin_User(string path);//文件写入内存用户信息
    void Writeout_User(string path);
    void Base_Start();//初始化
    void Base_End(string path, vector<string>c);//内存保存至文件
};
#endif // MAINWINDOW_H
