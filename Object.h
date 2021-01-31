#ifndef OBJECT_H
#define OBJECT_H

#include<string>
#include<vector>
#include<set>
using namespace std;
class Object {
private:
	int id;//对象唯一标识符
	string FileName;//对象标名称
	bool Read;//读
	int Source_Read;//read权限来源的id
	bool Delegate_Read;//是否允许传递
	bool Update;//更新
	int Source_Update;//权限来源,-1表明没有授权
	bool Delegate_Update;
	bool Insert;//插入
	bool Delegate_Insert;
	int Source_Insert;//权限来源
	bool Delete;//删除
	int Source_Delete;//权限来源
	bool Delegate_delete;
public:
	set<int>Delegate_Read_Pool;//授权read对象的id
	set<int>Delegate_Insert_Pool;//授权insert对象的id
	set<int>Delegate_Update_Pool;
	set<int>Delegate_Delete_Pool;
	Object(int);
	Object(string filename="", int id = 1,bool read = false, bool update = false, bool insert = false, bool _delete = false, bool Delegate_delete = false, bool Delegate_Insert = false, bool Delegate_Read = false, bool Delegate_Update = false, int Source_Read = -1, int Source_Update = -1, int Source_Insert = -1, int Source_Delete = -1);
	void Modify_Read(bool read);
	void Modify_Insert(bool insert);
	void Modify_Update(bool update);
	void Modify_Delete(bool _delete);
	void Modify_Source_Read(int s);
	void Modify_Source_Update(int s);
	void Modify_Source_Insert(int s);
	void Modify_Source_Delete(int s);
	void Modify_Delegate_Read(bool Delegate_Read);
	void Modify_Delegate_Update(bool Delegate_Update);
	void Modify_Delegate_Insert(bool Delegate_Insert);
	void Modify_Delegate_Delete(bool Delegate_delete);
	string Get_Filename();
	int Get_Id();
	int Get_Source_Read();
	int Get_Source_Update();
	int Get_Source_Insert();
	int Get_Source_Delete();
	bool Get_Read();
	bool Get_Update();
	bool Get_Insert();
	bool Get_Delete();
	bool Get_Delegate_Read();
	bool Get_Delegate_Update();
	bool Get_Delegate_Insert();
	bool Get_Delegate_Delete();
	friend istream & operator >> (istream& in, Object& obj);	//重载io流运算符
	~Object();
};

#endif // !OBJECT_H
