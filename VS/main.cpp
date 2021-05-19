#include<iostream>
#include<vector>
#include<string>
#include<windows.h>
#include<cstdlib>
#include<map>
#include<set>
#include"name.h"
#include"Object.h"
#include"User.h"
#include"Method.h"
using namespace std;
set<int>id_pool;			//id池，每个对象/用户唯一的身份标识符
vector<User>usergroup;		//用户
vector<Object>objectgroup;	//对象
map<int, int>r;				//映射
vector<string>cmd;			//命令
vector<string>cmd_temp;

int main() {
	system("title User Management System");		//设置Dos界面标题
	system("mode con cols=100 lines=25");		//设置窗口大小
	Base_Start();//文件写入
	system("cls");
	cout << "welcome to the CommandLine" << endl << "@ copy right 2020 by DJH" << endl << endl;
	string username = "##@Guest~>>>";
	int id = -1;
	while (true) {
		cout << username;
		string s = "";
		s.clear();
		getline(cin, s);
		Command(s,username,id);
	}
	return 0;
}
