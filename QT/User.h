#ifndef USER_H
#define USER_H
#include<vector>
#include<string>
#include"Object.h"
using namespace std;

class User {
private:
    int id;//用户唯一标识符
    string password;//密钥
    string Username;//用户名

public:
    vector<Object>OBJ;//对象池
    User();//默认构造函数
    User(vector<Object>obj);//重载构造函数
    User(string name, int id, string password);//重载构造函数
    User(string name, int id, string password, vector<Object>obj);//
    bool Check_Read(Object obj);
    bool Check_Update(Object obj);
    bool Check_Insert(Object obj);
    bool Check_Delete(Object obj);
    bool CA_Read(Object obj);
    bool CA_Update(Object obj);
    bool CA_Insert(Object obj);
    bool CA_Delete(Object obj);
    int Get_id();
    string Get_Username();
    string Get_password();
    int Find_Id(Object& obj);
    int DelegateRead(Object& obj, User& user, bool CA = false);
    int DelegateInsert(Object& obj, User& user, bool CA = false);
    int DelegateUpdate(Object& obj, User& user, bool CA = false);
    int DelegateDelete(Object& obj, User& user, bool CA = false);
    int EraseRead(User& user, Object& obj);
    int EraseInsert(User& user, Object& obj);
    int EraseUpdate(User& user, Object& obj);
    int EraseDelete(User& user, Object& obj);
    void Modify_password(string password);
    void Modify_Username(string username);
    friend istream& operator >> (istream& in, User& user);
    friend ostream& operator<<(ostream& out, User& user);
    ~User();
};

void Remove_Read(User& user, int id);
void Remove_Insert(User& user, int id);
void Remove_Update(User& user, int id);
void Remove_Delete(User& user, int id);
void Read_Chain(User& user, int id, vector<int>path);
void Insert_Chain(User& user, int id, vector<int>path);
void Update_Chain(User& user, int id, vector<int>path);
void Delete_Chain(User& user, int id, vector<int>path);
#endif // USER_H
