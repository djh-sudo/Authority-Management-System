#ifndef METHOD_H
#define METHOD_H

#include"User.h"
#include<string>
using namespace std;
void Inicial_Root(User& user);
void Show();
void Inicial_Relation();
void Inicial_Command();
string Information(int k);
void Help();
void Help(int k);
void trim(string& s);
void Clear();
void Exit(int id);
int User_Find(string username);
int Object_Find(string obj);
void Login(string& name, int& id);
void Login(string& name, int& id, string username, string password);
void Logout(string& name, int& id);
string Read(bool pre);
string Insert(bool pre);
string Update(bool pre);
string Delete(bool pre);
void Auth(int id, string x);
void Auth_User(string username);
string charactor(int k);
void Auth_Object(string obj);
void Show_Object();
int Grant(int id, string auth, string user, string obj, bool CA = false);
int Revoke(int id, string auth, string user, string obj);
void Chain(string user, string obj, string auth);
void Function(int k, string& name, int& id, string auth = "", string user = "", string obj = "");
void Command(string s,string& username,int&id);
void Readin_Object(string path);
void Readin_User(string path);
void Base_Start();
void Base_End(string path, vector<string>c);
#endif // !METHOD
