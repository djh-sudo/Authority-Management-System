#ifndef OBJECT_H
#define OBJECT_H

#include<string>
#include<vector>
#include<set>
using namespace std;
class Object {
private:
	int id;//����Ψһ��ʶ��
	string FileName;//���������
	bool Read;//��
	int Source_Read;//readȨ����Դ��id
	bool Delegate_Read;//�Ƿ�������
	bool Update;//����
	int Source_Update;//Ȩ����Դ,-1����û����Ȩ
	bool Delegate_Update;
	bool Insert;//����
	bool Delegate_Insert;
	int Source_Insert;//Ȩ����Դ
	bool Delete;//ɾ��
	int Source_Delete;//Ȩ����Դ
	bool Delegate_delete;
public:
	set<int>Delegate_Read_Pool;//��Ȩread�����id
	set<int>Delegate_Insert_Pool;//��Ȩinsert�����id
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
	friend istream & operator >> (istream& in, Object& obj);	//����io�������
	~Object();
};

#endif // !OBJECT_H
