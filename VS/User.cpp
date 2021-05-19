#include<iostream>
#include"User.h"
#include<map>
using namespace std;
extern vector<User>usergroup;
extern vector<Object>objectgroup;
extern map<int, int>r;
	User::User() {
		this->OBJ.clear();
	}
	User::User(vector<Object>obj) {
		this->OBJ.clear();
		for (int i = 0; i < obj.size(); i++)
			this->OBJ.push_back(obj[i]);
	}
	User::User(string name, int id, string password) {
		this->Username = name;
		this->password = password;
		this->id = id;
		this->OBJ.clear();
	}
	User::User(string name, int id, string password, vector<Object>obj) {
		this->Username = name;
		this->password = password;
		this->id = id;
		this->OBJ.clear();
		for (int i = 0; i < obj.size(); i++)
			this->OBJ.push_back(obj[i]);
	}
	bool User::Check_Read(Object obj) {//�Ƿ���readȨ��
		for (vector<Object>::iterator it = this->OBJ.begin(); it != this->OBJ.end(); it++) {
			if ((*it).Get_Id() == obj.Get_Id()) {
				return (*it).Get_Read();
			}
		}
		return false;
	}
	bool User::Check_Update(Object obj) {//��ѯ�Ƿ���UpdateȨ��
		for (vector<Object>::iterator it = this->OBJ.begin(); it != this->OBJ.end(); it++) {
			if ((*it).Get_Id() == obj.Get_Id()) {
				return (*it).Get_Update();
			}
		}
		return false;
	}
	bool User::Check_Insert(Object obj) {//��ѯ�Ƿ���InsertȨ��
		for (vector<Object>::iterator it = this->OBJ.begin(); it != this->OBJ.end(); it++) {
			if ((*it).Get_Id() == obj.Get_Id()) {
				return (*it).Get_Insert();
			}
		}
		return false;
	}
	bool User::Check_Delete(Object obj) {//��ѯ�Ƿ���DeleteȨ��
		for (vector<Object>::iterator it = this->OBJ.begin(); it != this->OBJ.end(); it++) {
			if ((*it).Get_Id() == obj.Get_Id()) {
				return (*it).Get_Delete();
			}
		}
		return false;
	}
	bool User::CA_Read(Object obj) {//�Ƿ����ת��readȨ��
		for (vector<Object>::iterator it = this->OBJ.begin(); it != this->OBJ.end(); it++)
			if ((*it).Get_Id() == obj.Get_Id()) {
				return (*it).Get_Delegate_Read();
			}
		return false;
	}
	bool User::CA_Update(Object obj) {//�Ƿ����ת��update
		for (vector<Object>::iterator it = this->OBJ.begin(); it != this->OBJ.end(); it++)
			if ((*it).Get_Id() == obj.Get_Id()) {
				return (*it).Get_Delegate_Update();
			}
		return false;
	}
	bool User::CA_Insert(Object obj) {//�Ƿ����ת��insert
		for (vector<Object>::iterator it = this->OBJ.begin(); it != this->OBJ.end(); it++)
			if ((*it).Get_Id() == obj.Get_Id()) {
				return (*it).Get_Delegate_Insert();
			}
		return false;
	}
	bool User::CA_Delete(Object obj) {//�Ƿ����ת��delete
		for (vector<Object>::iterator it = this->OBJ.begin(); it != this->OBJ.end(); it++)
			if ((*it).Get_Id() == obj.Get_Id()) {
				return (*it).Get_Delegate_Delete();
			}
		return false;
	}
	int User::Get_id() {
		return this->id;
	}
	string User::Get_Username() {
		return Username;
	}
	string User::Get_password() {
		return password;
	}
	int User::Find_Id(Object& obj) {
		for (vector<Object>::iterator it = this->OBJ.begin(); it != this->OBJ.end(); it++) {
			if ((*it).Get_Id() == obj.Get_Id())
				return distance(this->OBJ.begin(), it);
		}
		return -1;
	}
	int User::DelegateRead(Object& obj, User& user, bool CA ) {//Ĭ�ϲ�������
		//��������Ƿ���Obj��Ȩ��
		if (Check_Read(obj) && CA_Read(obj)) {//������Ȩ����
			for (vector<Object>::iterator it = user.OBJ.begin(); it != user.OBJ.end(); it++) {
				if (obj.Get_Id() == (*it).Get_Id()) {//�ҵ�����Ȩ�û���obj��id��
					int idd = distance(user.OBJ.begin(), it);//�õ�������OBJ�е�ƫ�Ƶ�ַ
					if (!user.OBJ[idd].Get_Read()) {//֮ǰû�б���Ȩ��,�ж�user��readȨ���Ƿ�Ϊfalse
						user.OBJ[idd].Modify_Read(true);//�õ���Ȩ
						user.OBJ[idd].Modify_Source_Read(this->Get_id());//�洢��Ȩ����id
						user.OBJ[idd].Modify_Delegate_Read(CA);//�洢��Ȩ�Ƿ�����ת�ñ�־
						this->OBJ[idd].Delegate_Read_Pool.insert(user.Get_id());//��Ȩ���洢�õ���Ȩ�ߵ�id
						cout << "��Ȩ�ɹ�!" << this->Get_Username() << "->" << user.Get_Username() << endl;;
						return 1;
					}
					else//read��־λtrue
					{
						int lid = user.OBJ[idd].Get_Source_Read();//�õ�ֱ����ȨԴ��id��
						if (lid == this->Get_id()) {
							if (CA != user.OBJ[idd].Get_Delegate_Read()) {
								user.OBJ[idd].Modify_Delegate_Read(CA);//�洢��Ȩ�Ƿ�����ת�ñ�־
								cout << "��Ȩ�ɹ�!" << endl;
								return 1;
							}
							else
								cout << "����Ȩ�������ظ�!" << endl;
							return 0;
						}
						if (usergroup[r[lid]].OBJ[idd].Delegate_Read_Pool.count(this->Get_id())) {
							//ֱ����ȨԴ����������Ȩ�ߣ���Ȩ��Ч
							cout << "Ȩ�޲���,��Ȩʧ��[read]" << this->Get_Username() << "->" << user.Get_Username() << endl;
							return 0;
						}
						else//������ֱ�Ӱ�����ϵ����Ҫ��������
						{
							int temp = lid;
							while (temp != 0) {
								temp = usergroup[r[temp]].OBJ[idd].Get_Source_Read();//���Ϸ�����ȨԴ
								if (temp == this->Get_id()) {//������Ȩ��Ȩ�޸���
									usergroup[r[lid]].OBJ[idd].Delegate_Read_Pool.erase(user.Get_id());//ԭ������ȨԴɾ��user��id
									this->OBJ[idd].Delegate_Read_Pool.insert(user.Get_id());
									user.OBJ[idd].Modify_Source_Read(this->Get_id());//user����ȨԴ�޸�Ϊ������Ȩ��
									user.OBJ[idd].Modify_Delegate_Read(CA);//��Ȩ���ݱ�־λȡ���ڱ�����Ȩ��
									cout << "��Ȩ�Ѹ���" << this->Get_Username() << "->" << user.Get_Username() << endl;
									return 1;
								}
							}
							cout << "Ȩ�޲��㣬��Ȩʧ��[read]" << endl;
						}
					}
				}
			}
			cout << "���Ҷ���OBJ������!" << endl;
		}
		else
			cout << "��Ȩ��Ȩ!" << endl;
		return 0;
	}
	int User::DelegateInsert(Object& obj, User& user, bool CA) {//Ĭ�ϲ�������
		//��������Ƿ���Obj��Ȩ��
		if (Check_Insert(obj) && CA_Insert(obj)) {//������Ȩ����
			for (vector<Object>::iterator it = user.OBJ.begin(); it != user.OBJ.end(); it++) {
				if (obj.Get_Id() == (*it).Get_Id()) {//�ҵ�����Ȩ�û���obj��id��
					int idd = distance(user.OBJ.begin(), it);//�õ�������OBJ�е�ƫ�Ƶ�ַ
					if (!user.OBJ[idd].Get_Insert()) {//֮ǰû�б���Ȩ��,�ж�user��readȨ���Ƿ�Ϊfalse
						user.OBJ[idd].Modify_Insert(true);//�õ���Ȩ
						user.OBJ[idd].Modify_Source_Insert(this->Get_id());//�洢��Ȩ����id
						user.OBJ[idd].Modify_Delegate_Insert(CA);//�洢��Ȩ�Ƿ�����ת�ñ�־
						this->OBJ[idd].Delegate_Insert_Pool.insert(user.Get_id());//��Ȩ���洢�õ���Ȩ�ߵ�id
						cout << "��Ȩ�ɹ�!" << this->Get_Username() << "->" << user.Get_Username() << endl;;
						return 1;
					}
					else//read��־λtrue
					{
						int lid = user.OBJ[idd].Get_Source_Insert();//�õ�ֱ����ȨԴ��id��
						if (lid == this->Get_id()) {
							if (CA != user.OBJ[idd].Get_Delegate_Insert()) {
								user.OBJ[idd].Modify_Delegate_Insert(CA);//�洢��Ȩ�Ƿ�����ת�ñ�־
								cout << "��Ȩ�ɹ�!" << endl;
								return 1;
							}
							else
								cout << "����Ȩ�������ظ�!" << endl;
							return 0;
						}
						if (usergroup[r[lid]].OBJ[idd].Delegate_Insert_Pool.count(this->Get_id())) {
							//ֱ����ȨԴ����������Ȩ�ߣ���Ȩ��Ч
							cout << "Ȩ�޲���,��Ȩʧ��[read]" << this->Get_Username() << "->" << user.Get_Username() << endl;
							return 0;
						}
						else//������ֱ�Ӱ�����ϵ����Ҫ��������
						{
							int temp = lid;
							while (temp != 0) {
								temp = usergroup[r[temp]].OBJ[idd].Get_Source_Insert();//���Ϸ�����ȨԴ
								if (temp == this->Get_id()) {//������Ȩ��Ȩ�޸���
									usergroup[r[lid]].OBJ[idd].Delegate_Insert_Pool.erase(user.Get_id());//ԭ������ȨԴɾ��user��id
									this->OBJ[idd].Delegate_Insert_Pool.insert(user.Get_id());
									user.OBJ[idd].Modify_Source_Insert(this->Get_id());//user����ȨԴ�޸�Ϊ������Ȩ��
									user.OBJ[idd].Modify_Delegate_Insert(CA);//��Ȩ���ݱ�־λȡ���ڱ�����Ȩ��
									cout << "��Ȩ�Ѹ���" << this->Get_Username() << "->" << user.Get_Username() << endl;
									return 1;
								}
							}
							cout << "Ȩ�޲��㣬��Ȩʧ��[read]" << endl;
						}
					}
				}
			}
			cout << "���Ҷ���OBJ������!" << endl;
		}
		else
			cout << "��Ȩ��Ȩ!" << endl;
		return 0;
	}
	int User::DelegateUpdate(Object& obj, User& user, bool CA) {//Ĭ�ϲ�������
	//��������Ƿ���Obj��Ȩ��
		if (Check_Update(obj) && CA_Update(obj)) {//������Ȩ����
			for (vector<Object>::iterator it = user.OBJ.begin(); it != user.OBJ.end(); it++) {
				if (obj.Get_Id() == (*it).Get_Id()) {//�ҵ�����Ȩ�û���obj��id��
					int idd = distance(user.OBJ.begin(), it);//�õ�������OBJ�е�ƫ�Ƶ�ַ
					if (!user.OBJ[idd].Get_Update()) {//֮ǰû�б���Ȩ��,�ж�user��readȨ���Ƿ�Ϊfalse
						user.OBJ[idd].Modify_Update(true);//�õ���Ȩ
						user.OBJ[idd].Modify_Source_Update(this->Get_id());//�洢��Ȩ����id
						user.OBJ[idd].Modify_Delegate_Update(CA);//�洢��Ȩ�Ƿ�����ת�ñ�־
						this->OBJ[idd].Delegate_Update_Pool.insert(user.Get_id());//��Ȩ���洢�õ���Ȩ�ߵ�id
						cout << "��Ȩ�ɹ�!" << this->Get_Username() << "->" << user.Get_Username() << endl;;
						return 1;
					}
					else//read��־λtrue
					{
						int lid = user.OBJ[idd].Get_Source_Update();//�õ�ֱ����ȨԴ��id��
						if (lid == this->Get_id()) {
							if (CA!=user.OBJ[idd].Get_Delegate_Update()) {
								user.OBJ[idd].Modify_Delegate_Update(CA);//�洢��Ȩ�Ƿ�����ת�ñ�־
								cout << "��Ȩ�ɹ�!" << endl;
								return 1;
							}
							else
								cout << "����Ȩ�������ظ�!" << endl;
							return 0;
						}
						if (usergroup[r[lid]].OBJ[idd].Delegate_Update_Pool.count(this->Get_id())) {
							//ֱ����ȨԴ����������Ȩ�ߣ���Ȩ��Ч
							cout << "Ȩ�޲���,��Ȩʧ��[read]" << this->Get_Username() << "->" << user.Get_Username() << endl;
							return 0;
						}
						else//������ֱ�Ӱ�����ϵ����Ҫ��������
						{
							int temp = lid;
							while (temp != 0) {
								temp = usergroup[r[temp]].OBJ[idd].Get_Source_Update();//���Ϸ�����ȨԴ
								if (temp == this->Get_id()) {//������Ȩ��Ȩ�޸���
									usergroup[r[lid]].OBJ[idd].Delegate_Update_Pool.erase(user.Get_id());//ԭ������ȨԴɾ��user��id
									this->OBJ[idd].Delegate_Update_Pool.insert(user.Get_id());
									user.OBJ[idd].Modify_Source_Update(this->Get_id());//user����ȨԴ�޸�Ϊ������Ȩ��
									user.OBJ[idd].Modify_Delegate_Update(CA);//��Ȩ���ݱ�־λȡ���ڱ�����Ȩ��
									cout << "��Ȩ�Ѹ���" << this->Get_Username() << "->" << user.Get_Username() << endl;
									return 1; 
								}
							}
							cout << "Ȩ�޲��㣬��Ȩʧ��[read]" << endl;
						}
					}
				}
			}
			cout << "���Ҷ���OBJ������!" << endl;
		}
		else
			cout << "��Ȩ��Ȩ!" << endl;
		return 0;
	}
	int User::DelegateDelete(Object& obj, User& user, bool CA) {//Ĭ�ϲ�������
	//��������Ƿ���Obj��Ȩ��
		if (Check_Delete(obj) && CA_Delete(obj)) {//������Ȩ����
			for (vector<Object>::iterator it = user.OBJ.begin(); it != user.OBJ.end(); it++) {
				if (obj.Get_Id() == (*it).Get_Id()) {//�ҵ�����Ȩ�û���obj��id��
					int idd = distance(user.OBJ.begin(), it);//�õ�������OBJ�е�ƫ�Ƶ�ַ
					if (!user.OBJ[idd].Get_Delete()) {//֮ǰû�б���Ȩ��,�ж�user��readȨ���Ƿ�Ϊfalse
						user.OBJ[idd].Modify_Delete(true);//�õ���Ȩ
						user.OBJ[idd].Modify_Source_Delete(this->Get_id());//�洢��Ȩ����id
						user.OBJ[idd].Modify_Delegate_Delete(CA);//�洢��Ȩ�Ƿ�����ת�ñ�־
						this->OBJ[idd].Delegate_Delete_Pool.insert(user.Get_id());//��Ȩ���洢�õ���Ȩ�ߵ�id
						cout << "��Ȩ�ɹ�!" << this->Get_Username() << "->" << user.Get_Username() << endl;;
						return 1;
					}
					else//read��־λtrue
					{
						int lid = user.OBJ[idd].Get_Source_Delete();//�õ�ֱ����ȨԴ��id��
						if (lid == this->Get_id()) {
							if (CA != user.OBJ[idd].Get_Delegate_Delete()) {
								user.OBJ[idd].Modify_Delegate_Delete(CA);//�洢��Ȩ�Ƿ�����ת�ñ�־
								cout << "��Ȩ�ɹ�!" << endl;
								return 1;
							}
							else
								cout << "����Ȩ�������ظ�!" << endl;
							return 0;
						}
						if (usergroup[r[lid]].OBJ[idd].Delegate_Delete_Pool.count(this->Get_id())) {
							//ֱ����ȨԴ����������Ȩ�ߣ���Ȩ��Ч
							cout << "Ȩ�޲���,��Ȩʧ��[read]" << this->Get_Username() << "->" << user.Get_Username() << endl;
							return 0;
						}
						else//������ֱ�Ӱ�����ϵ����Ҫ��������
						{
							int temp = lid;
							while (temp != 0) {
								temp = usergroup[r[temp]].OBJ[idd].Get_Source_Delete();//���Ϸ�����ȨԴ
								if (temp == this->Get_id()) {//������Ȩ��Ȩ�޸���
									usergroup[r[lid]].OBJ[idd].Delegate_Delete_Pool.erase(user.Get_id());//ԭ������ȨԴɾ��user��id
									this->OBJ[idd].Delegate_Delete_Pool.insert(user.Get_id());
									user.OBJ[idd].Modify_Source_Delete(this->Get_id());//user����ȨԴ�޸�Ϊ������Ȩ��
									user.OBJ[idd].Modify_Delegate_Delete(CA);//��Ȩ���ݱ�־λȡ���ڱ�����Ȩ��
									cout << "��Ȩ�Ѹ���" << this->Get_Username() << "->" << user.Get_Username() << endl;
									return 1;
								}
							}
							cout << "Ȩ�޲��㣬��Ȩʧ��[read]" << endl;
						}
					}
				}
			}
			cout << "���Ҷ���OBJ������!" << endl;
		}
		else
			cout << "��Ȩ��Ȩ!" << endl;
		return 0;
	}
	int User::EraseRead(User& user, Object& obj) {
		int id = Find_Id(obj);
		if (id == -1) {
			cout << "No object named " << obj.Get_Filename() << endl;
			return 0;
		}
		if (OBJ[id].Delegate_Read_Pool.erase(user.Get_id())) {
			Remove_Read(user, id);
			return 1;
		}
		if (Get_id() == 0) {
			Remove_Read(user, id);
			return 1;
		}
		else
			cout << "Ȩ�޲���,Ȩ�޻���ʧ��" << this->Get_Username() << "->" << user.Get_Username() << endl;
		return 0;
	}
	int User::EraseInsert(User& user, Object& obj) {
		int id = Find_Id(obj);
		if (id == -1) {
			cout << "No object named " << obj.Get_Filename() << endl;
			return 0;
		}
		if (OBJ[id].Delegate_Insert_Pool.erase(user.Get_id())) {
			Remove_Insert(user, id);
			return 1;
		}
		if (Get_id() == 0) {
			Remove_Insert(user, id);
			return 1;
		}
		else
			cout << "Ȩ�޲���,Ȩ�޻���ʧ��" << this->Get_Username() << "->" << user.Get_Username() << endl;
		return 0;
	}
	int User::EraseUpdate(User& user, Object& obj) {
		int id = Find_Id(obj);
		if (id == -1) {
			cout << "No object named " << obj.Get_Filename() << endl;
			return 0;
		}
		if (OBJ[id].Delegate_Update_Pool.erase(user.Get_id())) {
			Remove_Update(user, id);
			return 1;
		}
		if (Get_id() == 0) {
			Remove_Update(user, id);
			return 1;
		}
		else
			cout << "Ȩ�޲���,Ȩ�޻���ʧ��" << this->Get_id() << "->" << user.Get_id() << endl;
		return 0;
	}
	int User::EraseDelete(User& user, Object& obj) {
		int id = Find_Id(obj);
		if (id == -1) {
			cout << "No object named " << obj.Get_Filename() << endl;
			return 0;
		}
		if (OBJ[id].Delegate_Delete_Pool.erase(user.Get_id())) {
			Remove_Delete(user, id);
			return 1;
		}
		if (Get_id() == 0) {
			Remove_Delete(user, id);
			return 1;
		}
		else
			cout << "Ȩ�޲���,Ȩ�޻���ʧ��" << this->Get_id() << "->" << user.Get_id() << endl;
	}
	void User::Modify_password(string password) {
		this->password = password;
	}
	void User::Modify_Username(string username) {
		this->Username = username;
	}
	User::~User() {}
	istream& operator >> (istream& in, User& user) {
		in >> user.id >> user.password>>user.Username;
		return in;
	}
	void Remove_Read(User& user, int id) {
		user.OBJ[id].Modify_Source_Read(-1);
		user.OBJ[id].Modify_Read(false);
		user.OBJ[id].Modify_Delegate_Read(false);
		if (user.OBJ[id].Delegate_Read_Pool.empty()) {
			return;
		}
		for (set<int>::iterator it = user.OBJ[id].Delegate_Read_Pool.begin(); it != user.OBJ[id].Delegate_Read_Pool.end(); it++) {
			Remove_Read(usergroup[r[*it]], id);
		}
		user.OBJ[id].Delegate_Read_Pool.clear();
	}
	void Remove_Insert(User& user, int id) {
		user.OBJ[id].Modify_Source_Insert(-1);
		user.OBJ[id].Modify_Insert(false);
		user.OBJ[id].Modify_Delegate_Insert(false);
		if (user.OBJ[id].Delegate_Insert_Pool.empty()) {
			return;
		}
		for (set<int>::iterator it = user.OBJ[id].Delegate_Insert_Pool.begin(); it != user.OBJ[id].Delegate_Insert_Pool.end(); it++) {
			Remove_Insert(usergroup[r[*it]], id);
		}
		user.OBJ[id].Delegate_Insert_Pool.clear();
	}
	void Remove_Update(User& user, int id) {
		user.OBJ[id].Modify_Source_Update(-1);
		user.OBJ[id].Modify_Update(false);
		user.OBJ[id].Modify_Delegate_Update(false);
		if (user.OBJ[id].Delegate_Update_Pool.empty()) {
			return;
		}
		for (set<int>::iterator it = user.OBJ[id].Delegate_Update_Pool.begin(); it != user.OBJ[id].Delegate_Update_Pool.end(); it++) {
			Remove_Update(usergroup[r[*it]], id);
		}
		user.OBJ[id].Delegate_Update_Pool.clear();
	}
	void Remove_Delete(User& user, int id) {
		user.OBJ[id].Modify_Source_Delete(-1);
		user.OBJ[id].Modify_Delete(false);
		user.OBJ[id].Modify_Delegate_Delete(false);
		if (user.OBJ[id].Delegate_Delete_Pool.empty()) {
			return;
		}
		for (set<int>::iterator it = user.OBJ[id].Delegate_Delete_Pool.begin(); it != user.OBJ[id].Delegate_Delete_Pool.end(); it++) {
			Remove_Delete(usergroup[r[*it]], id);
		}
		user.OBJ[id].Delegate_Delete_Pool.clear();
	}
	void Read_Chain(User& user, int id, vector<int>path) {
		path.push_back(user.Get_id());
		int flag = user.OBJ[id].Delegate_Read_Pool.empty();
		if (flag) {//���Ӽ���Ϊ��
			int i;
			for (i = 0; i < path.size() - 1; i++) {
				cout << usergroup[r[path[i]]].Get_Username() << "->";
			}
			cout << usergroup[r[path[i]]].Get_Username() << endl;
			return;
		}
		else//�ּ��ϲ�Ϊ��
		{
			for (set<int>::iterator it = user.OBJ[id].Delegate_Read_Pool.begin(); it != user.OBJ[id].Delegate_Read_Pool.end(); it++) {
				Read_Chain(usergroup[r[*it]], id, path);
			}
		}
	}
	void Insert_Chain(User& user, int id, vector<int>path) {
		path.push_back(user.Get_id());
		int flag = user.OBJ[id].Delegate_Insert_Pool.empty();
		if (flag) {//���Ӽ���Ϊ��
			int i;
			for (i = 0; i < path.size() - 1; i++) {
				cout << usergroup[r[path[i]]].Get_Username() << "->";
			}
			cout << usergroup[r[path[i]]].Get_Username() << endl;
			return;
		}
		else//�ּ��ϲ�Ϊ��
		{
			for (set<int>::iterator it = user.OBJ[id].Delegate_Insert_Pool.begin(); it != user.OBJ[id].Delegate_Insert_Pool.end(); it++) {
				Insert_Chain(usergroup[r[*it]], id, path);
			}
		}
	}
	void Update_Chain(User& user, int id, vector<int>path) {
		path.push_back(user.Get_id());
		int flag = user.OBJ[id].Delegate_Update_Pool.empty();
		if (flag) {//���Ӽ���Ϊ��
			int i;
			for (i = 0; i < path.size() - 1; i++) {
				cout << usergroup[r[path[i]]].Get_Username() << "->";
			}
			cout << usergroup[r[path[i]]].Get_Username() << endl;
			return;
		}
		else//�ּ��ϲ�Ϊ��
		{
			for (set<int>::iterator it = user.OBJ[id].Delegate_Update_Pool.begin(); it != user.OBJ[id].Delegate_Update_Pool.end(); it++) {
				Update_Chain(usergroup[r[*it]], id, path);
			}
		}
	}
	void Delete_Chain(User& user, int id, vector<int>path) {
		path.push_back(user.Get_id());
		int flag = user.OBJ[id].Delegate_Delete_Pool.empty();
		if (flag) {//���Ӽ���Ϊ��
			int i;
			for (i = 0; i < path.size() - 1; i++) {
				cout << usergroup[r[path[i]]].Get_Username() << "->";
			}
			cout << usergroup[r[path[i]]].Get_Username() << endl;
			return;
		}
		else//�ּ��ϲ�Ϊ��
		{
			for (set<int>::iterator it = user.OBJ[id].Delegate_Delete_Pool.begin(); it != user.OBJ[id].Delegate_Delete_Pool.end(); it++) {
				Delete_Chain(usergroup[r[*it]], id, path);
			}
		}
	}
