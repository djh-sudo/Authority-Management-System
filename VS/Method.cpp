#include<iostream>
#include<vector>
#include<string>
#include<conio.h>
#include"Method.h"
#include"User.h"
#include"name.h"
#include<algorithm>
#include<fstream>
#include<cstdlib>
#include<map>
using namespace std;
extern vector<User>usergroup;
extern vector<Object>objectgroup;
extern map<int, int>r;
extern vector<string>cmd;
extern vector<string>cmd_temp;
int flag = 0;
void Inicial_Root(User& user) {//引用问题
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
void Inicial_Relation() {
	for (int i = 0; i < usergroup.size(); i++) {
		r.insert(map<int, int>::value_type(usergroup[i].Get_id(), i));
	}
}
void Inicial_Command() {
	cmd.push_back("help");
	cmd.push_back("log");
	cmd.push_back("exit");
	cmd.push_back("clc");
	cmd.push_back("out");
	/*
	cmd.push_back("auth");
	cmd.push_back("show object");
	*/
}
string Information(int k) {
	switch (k)
	{
	case 0: return "Do you mean 'help'?[this command will give you information]\n";
	case 1:	return "Do you mean 'log'?[log on the system]\n";
	case 2: return "Do you mean 'exit'?[exit from the system]\n";
	case 3: return "Do you mean 'clc'?[clear all]\n";
	case 4: return "Do you mean 'show object'?[show all the object]\n";
	case 5: return "Do you mean 'show auth'?[what authentication you have]\n";
	default:return "No command,you can use the 'help'\n";
	}
}
void Help() {
	cout << "welcome use the help,if you have any question" << endl;
	cout << "please send e-mail to djh113@126.com" << endl << endl;
	cout << "[help]  ## give you all command and information" << endl;
	cout << "[log]  ## log on this system with username and password" << endl;
	cout << "[exit]  ## exit from the system" << endl;
	cout << "[clc]  ## clear the screen,clear all thing" << endl;
	cout << "[out]  ##logout the system" << endl;
}
void Help(int k) {
	Help();
	if (k == -1) return;
	cout << "[auth -X]  ##  show authentication you have about object X" << endl;
	cout << "[show obj]  ## show all object in the system" << endl;
	if (k == 0) {
		cout << "[show auth of user -X]  ##  show the auth of the user X" << endl;
		cout << "[show auth of obj -X]  ##  show the object X with all users" << endl;
		cout << "[grant auth -X of -O to -U]  ##  grant the X auth of Object O to User U\n The X should be read/upd/del/ins" << endl;
		cout << "[revoke auth -X of -O from -U]  ##  revoke the X auth of Oject O from User U\n The X should be read/upd/del/ins" << endl;
		cout << "[revoke auth -X of -O]  ##  revoke X auth of object O from all users ";
	}
}
void trim(string& s) {//字符串去掉空格
	int index = 0;
	if (!s.empty()) {
		while ((index = s.find(' ', index)) != string::npos) {
			s.erase(index, 1);
		}
	}
}
void Clear() {
	system("cls");
	cout << "welcome to the commandline" << endl << "@copy right 2020 by djh" << endl << endl;
}
void Exit(int id) {
	Base_End(md(usergroup[r[id]].Get_Username()), cmd_temp);
	exit(0);
}
int User_Find(string username) {//根据用户名返回用户的id
	for (int i = 0; i < usergroup.size(); i++) {
		string s;
		s.clear();
		s = usergroup[i].Get_Username();
		transform(s.begin(), s.end(), s.begin(), ::tolower);//大小写不敏感
		if (s.find(username) != string::npos) {
			if (s == username) {
				return usergroup[i].Get_id();
			}
			else
				return ~usergroup[i].Get_id();
		}
	}
	return -999;
}
int Object_Find(string obj) {
	for (int i = 0; i < objectgroup.size(); i++) {
		string s;
		s.clear();
		s = objectgroup[i].Get_Filename();
		transform(s.begin(), s.end(), s.begin(), ::tolower);//大小写不敏感
		if (s.find(obj) != string::npos) {
			if (s == obj) {
				return i;
			}
			else
				return ~i;
		}
	}
	return -99999;
}
void Login(string& name, int& id) {
	if (id == -1) {
		cmd_temp.clear();
		string username;
		string password;
		username.clear();
		password.clear();
		cout << "the username: ";
		cin >> username;
		for (int i = 0; i < usergroup.size(); i++) {
			if (usergroup[i].Get_Username() == username) {//找到用户名
				cout << "the password: ";
				char ch;
				while ((ch = getch()) != 13) {//输入密码，遇到回车结束
					password += ch;
					cout << "*";
				}
				if (md(password) == usergroup[i].Get_password()) {
					name = "##@" + usergroup[i].Get_Username() + "~>>>";
					id = usergroup[i].Get_id();
					cmd.push_back("auth-");
					cmd.push_back("showobj");
					cmd.push_back("grantauth-of-to-");
					cmd.push_back("revokeauth-of-from-");
					cmd.push_back("grantauth-of-to-withoption");
					if (id == 0) {
						cmd.push_back("showauthofuser-");
						cmd.push_back("showauthofobj-");
						cmd.push_back("chain-of-");
						//cmd.push_back("revokeauth-of-");
					}
					cout << endl << "logon this system successfully" << endl;
					return;
				}
				cout << "error with the username or password" << endl;
				return;
			}
		}
		cout << "the password: ";
		char ch;
		while ((ch = getch()) != 13) {//输入密码，遇到回车结束
			password += ch;
			cout << "*";
		}
		name = "##@Guest~>>>";
		cout << endl << "error with the username or password" << endl;
		return;
	}
	cout << "you have log on the system" << endl;
}

void Login(string& name, int& id,string username,string password) {
	if (id == -1) {
		for (int i = 0; i < usergroup.size(); i++) {
			if (usergroup[i].Get_Username() == username) {//找到用户名
				if (password == usergroup[i].Get_password()) {
					name = "##@" + usergroup[i].Get_Username() + "~>>>";
					id = usergroup[i].Get_id();
					cmd.push_back("auth-");
					cmd.push_back("showobj");
					cmd.push_back("grantauth-of-to-");
					cmd.push_back("revokeauth-of-from-");
					cmd.push_back("grantauth-of-to-withoption");
					if (id == 0) {
						cmd.push_back("showauthofuser-");
						cmd.push_back("showauthofobj-");
						cmd.push_back("chain-of-");
					}
					//cout << endl << "logon this system successfully" << endl;
					return;
				}
				//cout << "error with the username or password" << endl;
				return;
			}
		}
		name = "##@Guest~>>>";
		//cout << endl << "error with the username or password" << endl;
		return;
	}
	//cout << "you have log on the system" << endl;
}
void Logout(string& name, int& id) {
	if (id != -1) {
		id = -1;
		name = "##@Guest~>>>";
		while (cmd.size() != 5)
			cmd.pop_back();
		Clear();
		cout << "you have log out the system" << endl;
		Base_End(md(usergroup[r[id]].Get_Username()), cmd_temp);
	}
	else
		cout << "you have not log on this system" << endl;
}
string Read(bool pre) {
	if (pre)
		return "read";
	else
		return "";
}
string Insert(bool pre) {
	if (pre)
		return "ins";
	else
		return "";
}
string Update(bool pre) {
	if (pre)
		return "upda";
	else
		return "";
}
string Delete(bool pre) {
	if (pre)
		return "dele";
	else
		return"";
}
void Auth(int id, string x) {//根用户的id查看授权信息,仅供查询自己
//根据Object的名字查找obj的下标
	for (int i = 0; i < objectgroup.size(); i++) {
		string s = objectgroup[i].Get_Filename();//大小写不敏感
		transform(s.begin(), s.end(), s.begin(), ::tolower);
		if (x == s) {
			if (usergroup[r[id]].OBJ[i].Get_Read())
				cout << Read(usergroup[r[id]].OBJ[i].Get_Read()) << charactor(usergroup[r[id]].OBJ[i].Get_Delegate_Read()) << " ";
			if (usergroup[r[id]].OBJ[i].Get_Insert())
				cout << Insert(usergroup[r[id]].OBJ[i].Get_Insert()) << charactor(usergroup[r[id]].OBJ[i].Get_Delegate_Insert()) << " ";
			if (usergroup[r[id]].OBJ[i].Get_Update())
				cout << Update(usergroup[r[id]].OBJ[i].Get_Update()) << charactor(usergroup[r[id]].OBJ[i].Get_Delegate_Update()) << " ";
			if (usergroup[r[id]].OBJ[i].Get_Delete())
				cout << Delete(usergroup[r[id]].OBJ[i].Get_Delete()) << charactor(usergroup[r[id]].OBJ[i].Get_Delegate_Delete());
			cout << endl;
			return;
		}
	}
	cout << "No object named " << x << endl;
	return;
}
void Auth_User(string username) {//root查询用户的授权情况
	int index = User_Find(username);
	if (index == -999) {
		cout << "No user named " << username << endl;
		return;
	}
	if (index < 0) {
		cout << "Do you find user " << usergroup[r[~index]].Get_Username() << "?" << endl;
		return;
	}
	else {
		for (int i = 0; i < objectgroup.size(); i++) {
			cout << objectgroup[i].Get_Filename() << ":";
			if (usergroup[r[index]].OBJ[i].Get_Read())
				cout << Read(usergroup[r[index]].OBJ[i].Get_Read()) << "[" << usergroup[r[usergroup[r[index]].OBJ[i].Get_Source_Read()]].Get_Username() << "]" << charactor(usergroup[r[index]].OBJ[i].Get_Delegate_Read()) << " ";
			if (usergroup[r[index]].OBJ[i].Get_Insert())
				cout << Insert(usergroup[r[index]].OBJ[i].Get_Insert()) << "[" << usergroup[r[usergroup[r[index]].OBJ[i].Get_Source_Insert()]].Get_Username() << "]" << charactor(usergroup[r[index]].OBJ[i].Get_Delegate_Insert()) << " ";
			if (usergroup[r[index]].OBJ[i].Get_Update())
				cout << Update(usergroup[r[index]].OBJ[i].Get_Update()) << "[" << usergroup[r[usergroup[r[index]].OBJ[i].Get_Source_Update()]].Get_Username() << "]" << charactor(usergroup[r[index]].OBJ[i].Get_Delegate_Update()) << " ";
			if (usergroup[r[index]].OBJ[i].Get_Delete())
				cout << Delete(usergroup[r[index]].OBJ[i].Get_Delete()) << "[" << usergroup[r[usergroup[r[index]].OBJ[i].Get_Source_Delete()]].Get_Username() << "]" << charactor(usergroup[r[index]].OBJ[i].Get_Delegate_Delete()) << " ";
			cout << endl;
		}
	}
}
string charactor(int k) {
	if (k)
		return "+";
	else
		return "-";
}
void Auth_Object(string obj) {//root查询某个对象的授权情况
	int index = Object_Find(obj);//obj在pool中的下标
	if (index == -99999) {
		cout << "No object named " << obj << endl;
		return;
	}
	if (index < 0) {
		cout << "Do you find object " << objectgroup[~index].Get_Filename() << "?" << endl;
		return;
	}
	else
	{
		cout << "Read: ";
		for (int i = 0; i < usergroup.size(); i++) {
			if (usergroup[i].OBJ[index].Get_Read())
				cout << usergroup[i].Get_Username() << "[" << usergroup[r[usergroup[i].OBJ[index].Get_Source_Read()]].Get_Username() << "]" << charactor(usergroup[i].OBJ[index].Get_Delegate_Read()) << " ";
		}
		cout << endl << "Insert: ";
		for (int i = 0; i < usergroup.size(); i++) {
			if (usergroup[i].OBJ[index].Get_Insert())
				cout << usergroup[i].Get_Username() << "[" << usergroup[r[usergroup[i].OBJ[index].Get_Source_Insert()]].Get_Username() << "]" << charactor(usergroup[i].OBJ[index].Get_Delegate_Insert()) << " ";
		}
		cout << endl << "Update: ";
		for (int i = 0; i < usergroup.size(); i++) {
			if (usergroup[i].OBJ[index].Get_Update())
				cout << usergroup[i].Get_Username() << "[" << usergroup[r[usergroup[i].OBJ[index].Get_Source_Update()]].Get_Username() << "]" << charactor(usergroup[i].OBJ[index].Get_Delegate_Update()) << " ";
		}
		cout << endl << "Delete: ";
		for (int i = 0; i < usergroup.size(); i++) {
			if (usergroup[i].OBJ[index].Get_Delete())
				cout << usergroup[i].Get_Username() << "[" << usergroup[r[usergroup[i].OBJ[index].Get_Source_Delete()]].Get_Username() << "]" << charactor(usergroup[i].OBJ[index].Get_Delegate_Delete()) << " ";
		}
		cout << endl;
	}
}
void Show_Object() {
	for (int i = 0; i < objectgroup.size(); i++)
		cout << objectgroup[i].Get_Filename() << " [" << objectgroup[i].Get_Id() << "] ";
	cout << endl;
	return;
}
int Grant(int id, string auth, string user, string obj, bool CA) {//授权者的id,授权操作，被授权用户名，授权对象名
	int uid = User_Find(user);
	if (uid == -999) {
		cout << "No named " << user << endl;
		return 0;
	}
	if (uid < 0) {
		cout << "Do you find user " << usergroup[r[~uid]].Get_Username() << "?" << endl;
		return 0;
	}
	int oid = Object_Find(obj);
	if (oid == -99999) {
		cout << "No object named " << obj << endl;
		return 0;
	}
	if (oid < 0) {
		cout << "Do you find object " << objectgroup[~oid].Get_Filename() << "?" << endl;
		return 0;
	}
	if (auth == "read") {
		return (usergroup[r[id]].DelegateRead(objectgroup[oid], usergroup[r[uid]], CA));
	}
	if (auth == "upd") {
		return(usergroup[r[id]].DelegateUpdate(objectgroup[oid], usergroup[r[uid]], CA));
		
	}
	if (auth == "del") {
		return (usergroup[r[id]].DelegateDelete(objectgroup[oid], usergroup[r[uid]], CA));
	}
	if (auth == "ins") {
		return	(usergroup[r[id]].DelegateInsert(objectgroup[oid], usergroup[r[uid]], CA));
	}
	cout << "the premeter X must be one of the[read upd del ins]" << endl;
	return 0;
}
int Revoke(int id, string auth, string user, string obj) {
	int uid = User_Find(user);
	if (uid == -999) {
		cout << "No named " << user << endl;
		return 0;
	}
	if (uid < 0) {
		cout << "Do you find user " << usergroup[r[~uid]].Get_Username() << "?" << endl;
		return 0;
	}
	int oid = Object_Find(obj);
	if (oid == -99999) {
		cout << "No object named " << obj << endl;
		return 0;
	}
	if (oid < 0) {
		cout << "Do you find object " << objectgroup[~oid].Get_Filename() << "?" << endl;
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
	cout << "the premeter X must be one of the[read upd del ins]" << endl;
	return 0;
}
void Chain(string user, string obj, string auth) {
	vector<int>path;
	path.clear();
	int oid = Object_Find(obj);
	if (oid == -99999) {
		cout << "No object named " << obj << endl;
		return;
	}
	if (oid < 0) {
		cout << "Do you find object " << objectgroup[~oid].Get_Filename() << "?" << endl;
		return;
	}
	if (auth == "read") {
		cout << objectgroup[oid].Get_Filename() << " read:";
		Read_Chain(usergroup[0], oid, path);
		return;
	}
	if (auth == "del") {
		cout << objectgroup[oid].Get_Filename() << " delete:";
		Delete_Chain(usergroup[0], oid, path);
		return;
	}
	if (auth == "upd") {
		cout << objectgroup[oid].Get_Filename() << " update:";
		Update_Chain(usergroup[0], oid, path);
		return;
	}
	if (auth == "ins") {
		cout << objectgroup[oid].Get_Filename() << " insert:";
		Insert_Chain(usergroup[0], oid, path);
		return;
	}
	cout << "the premeter X must be one of the[read upd del ins]" << endl;
	return;
}
void Function(int k, string& name, int& id, string auth , string user, string obj) {//对命令进行译码操作
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
		if (Revoke(id, auth, user, obj)&&flag) {
			cmd_temp.push_back("revoke auth -" + auth + " of -" + obj + " from -" + user);
			//写入文件
		}break;
	}
	case 9: {//with option
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
	default:
		break;
	}
}
void Command(string s, string& username, int& id){
		transform(s.begin(), s.end(), s.begin(), ::tolower);//大小写不敏感
		int i = 0;
		for (i; i < cmd.size(); i++) {
			if (i < 5) {//不带参数的基本命令
				if (s.find(cmd[i]) != string::npos || cmd[i].find(s) != string::npos) {
					if (s == cmd[i]) {
						Function(i, username, id);
						break;
					}
					else
					{
						if (s != "") {
							cout << Information(i) << endl;
							break;
						}
						break;
					}
				}
			}
			else {//带参数的命令
				string copy;
				copy = s;
				vector<int>index;
				vector<int>padding;
				index.clear();
				padding.clear();
				for (int k = 0; k < copy.size(); k++) {
					if (copy[k] == '-') {
						index.push_back(k);
					}
					int flag = 90;
					if (!index.empty())
						flag = index.back();
					if (copy[k] == ' ' && k > flag && index.size() > padding.size()) {//获取-后的第一个空格
						padding.push_back(k);
					}
				}
				while (index.size() < 3) {
					index.push_back(copy.size() - 1);
				}
				while (padding.size() < 3) {
					padding.push_back(copy.size() - 1);
				}
				string pre1 = copy.substr(index[0] + 1, padding[0] - index[0]);//第一个参数
				string pre2 = copy.substr(index[1] + 1, padding[1] - index[1]);//第二个参数
				string pre3 = copy.substr(index[2] + 1, padding[2] - index[2]);//第三个参数
				int pos;
				trim(copy);
				trim(pre1);
				trim(pre2);
				trim(pre3);
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
				if (cmd[i] == copy) {
					Function(i, username, id, pre1, pre3, pre2);//id是登录者的id
					break;
				}
				else;
			}
		}
		if (i == cmd.size())
			cout << "there is no command with " << s << endl;
		return;
}
void Readin_Object(string path) {
	ifstream in(path.data());
	if (!in.is_open()) {
		cout << "Error opening file"; exit(1);
	}
	Object temp;
	while (in >> temp) {
		objectgroup.push_back(temp);
	}
	in.close();
	return;
}
void Readin_User(string path) {
	ifstream in(path.data());
	if (!in.is_open()) {
		cout << "Error opening file"; exit(1);
	}
	User temp(objectgroup);
	while (in >> temp) {
		usergroup.push_back(temp);
	}
	in.close();
}
void Base_Start() {
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
	for (int i = 0; i < usergroup.size(); i++) {
		string buffer = "";
		fcmd.clear();
		ifstream in(md(usergroup[i].Get_Username()));
		if (!in.is_open()) {
			cout << "Error opening file"; exit(1);
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
void Base_End(string path, vector<string>c) {
	ofstream out(path, ios::app);
	if (out.is_open())
	{
		for (int i = 0; i < c.size(); i++)
			out << endl << c[i];
		out.close();
	}
}