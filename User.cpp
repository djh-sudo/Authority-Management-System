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
	bool User::Check_Read(Object obj) {//是否有read权限
		for (vector<Object>::iterator it = this->OBJ.begin(); it != this->OBJ.end(); it++) {
			if ((*it).Get_Id() == obj.Get_Id()) {
				return (*it).Get_Read();
			}
		}
		return false;
	}
	bool User::Check_Update(Object obj) {//查询是否有Update权限
		for (vector<Object>::iterator it = this->OBJ.begin(); it != this->OBJ.end(); it++) {
			if ((*it).Get_Id() == obj.Get_Id()) {
				return (*it).Get_Update();
			}
		}
		return false;
	}
	bool User::Check_Insert(Object obj) {//查询是否有Insert权限
		for (vector<Object>::iterator it = this->OBJ.begin(); it != this->OBJ.end(); it++) {
			if ((*it).Get_Id() == obj.Get_Id()) {
				return (*it).Get_Insert();
			}
		}
		return false;
	}
	bool User::Check_Delete(Object obj) {//查询是否有Delete权限
		for (vector<Object>::iterator it = this->OBJ.begin(); it != this->OBJ.end(); it++) {
			if ((*it).Get_Id() == obj.Get_Id()) {
				return (*it).Get_Delete();
			}
		}
		return false;
	}
	bool User::CA_Read(Object obj) {//是否可以转授read权限
		for (vector<Object>::iterator it = this->OBJ.begin(); it != this->OBJ.end(); it++)
			if ((*it).Get_Id() == obj.Get_Id()) {
				return (*it).Get_Delegate_Read();
			}
		return false;
	}
	bool User::CA_Update(Object obj) {//是否可以转让update
		for (vector<Object>::iterator it = this->OBJ.begin(); it != this->OBJ.end(); it++)
			if ((*it).Get_Id() == obj.Get_Id()) {
				return (*it).Get_Delegate_Update();
			}
		return false;
	}
	bool User::CA_Insert(Object obj) {//是否可以转让insert
		for (vector<Object>::iterator it = this->OBJ.begin(); it != this->OBJ.end(); it++)
			if ((*it).Get_Id() == obj.Get_Id()) {
				return (*it).Get_Delegate_Insert();
			}
		return false;
	}
	bool User::CA_Delete(Object obj) {//是否可以转让delete
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
	int User::DelegateRead(Object& obj, User& user, bool CA ) {//默认不允许传递
		//检查自身是否有Obj的权限
		if (Check_Read(obj) && CA_Read(obj)) {//存在授权条件
			for (vector<Object>::iterator it = user.OBJ.begin(); it != user.OBJ.end(); it++) {
				if (obj.Get_Id() == (*it).Get_Id()) {//找到待授权用户的obj的id号
					int idd = distance(user.OBJ.begin(), it);//得到对象在OBJ中的偏移地址
					if (!user.OBJ[idd].Get_Read()) {//之前没有被授权过,判断user的read权限是否为false
						user.OBJ[idd].Modify_Read(true);//得到授权
						user.OBJ[idd].Modify_Source_Read(this->Get_id());//存储授权方的id
						user.OBJ[idd].Modify_Delegate_Read(CA);//存储授权是否允许转让标志
						this->OBJ[idd].Delegate_Read_Pool.insert(user.Get_id());//授权方存储得到授权者的id
						cout << "授权成功!" << this->Get_Username() << "->" << user.Get_Username() << endl;;
						return 1;
					}
					else//read标志位true
					{
						int lid = user.OBJ[idd].Get_Source_Read();//得到直接授权源的id号
						if (lid == this->Get_id()) {
							if (CA != user.OBJ[idd].Get_Delegate_Read()) {
								user.OBJ[idd].Modify_Delegate_Read(CA);//存储授权是否允许转让标志
								cout << "授权成功!" << endl;
								return 1;
							}
							else
								cout << "已授权，无需重复!" << endl;
							return 0;
						}
						if (usergroup[r[lid]].OBJ[idd].Delegate_Read_Pool.count(this->Get_id())) {
							//直接授权源包含本次授权者，授权无效
							cout << "权限不足,授权失败[read]" << this->Get_Username() << "->" << user.Get_Username() << endl;
							return 0;
						}
						else//不存在直接包含关系，需要向上索引
						{
							int temp = lid;
							while (temp != 0) {
								temp = usergroup[r[temp]].OBJ[idd].Get_Source_Read();//向上访问授权源
								if (temp == this->Get_id()) {//本次授权者权限更高
									usergroup[r[lid]].OBJ[idd].Delegate_Read_Pool.erase(user.Get_id());//原来的授权源删除user的id
									this->OBJ[idd].Delegate_Read_Pool.insert(user.Get_id());
									user.OBJ[idd].Modify_Source_Read(this->Get_id());//user的授权源修改为本次授权者
									user.OBJ[idd].Modify_Delegate_Read(CA);//授权传递标志位取决于本次授权者
									cout << "授权已覆盖" << this->Get_Username() << "->" << user.Get_Username() << endl;
									return 1;
								}
							}
							cout << "权限不足，授权失败[read]" << endl;
						}
					}
				}
			}
			cout << "查找对象OBJ不存在!" << endl;
		}
		else
			cout << "无权授权!" << endl;
		return 0;
	}
	int User::DelegateInsert(Object& obj, User& user, bool CA) {//默认不允许传递
		//检查自身是否有Obj的权限
		if (Check_Insert(obj) && CA_Insert(obj)) {//存在授权条件
			for (vector<Object>::iterator it = user.OBJ.begin(); it != user.OBJ.end(); it++) {
				if (obj.Get_Id() == (*it).Get_Id()) {//找到待授权用户的obj的id号
					int idd = distance(user.OBJ.begin(), it);//得到对象在OBJ中的偏移地址
					if (!user.OBJ[idd].Get_Insert()) {//之前没有被授权过,判断user的read权限是否为false
						user.OBJ[idd].Modify_Insert(true);//得到授权
						user.OBJ[idd].Modify_Source_Insert(this->Get_id());//存储授权方的id
						user.OBJ[idd].Modify_Delegate_Insert(CA);//存储授权是否允许转让标志
						this->OBJ[idd].Delegate_Insert_Pool.insert(user.Get_id());//授权方存储得到授权者的id
						cout << "授权成功!" << this->Get_Username() << "->" << user.Get_Username() << endl;;
						return 1;
					}
					else//read标志位true
					{
						int lid = user.OBJ[idd].Get_Source_Insert();//得到直接授权源的id号
						if (lid == this->Get_id()) {
							if (CA != user.OBJ[idd].Get_Delegate_Insert()) {
								user.OBJ[idd].Modify_Delegate_Insert(CA);//存储授权是否允许转让标志
								cout << "授权成功!" << endl;
								return 1;
							}
							else
								cout << "已授权，无需重复!" << endl;
							return 0;
						}
						if (usergroup[r[lid]].OBJ[idd].Delegate_Insert_Pool.count(this->Get_id())) {
							//直接授权源包含本次授权者，授权无效
							cout << "权限不足,授权失败[read]" << this->Get_Username() << "->" << user.Get_Username() << endl;
							return 0;
						}
						else//不存在直接包含关系，需要向上索引
						{
							int temp = lid;
							while (temp != 0) {
								temp = usergroup[r[temp]].OBJ[idd].Get_Source_Insert();//向上访问授权源
								if (temp == this->Get_id()) {//本次授权者权限更高
									usergroup[r[lid]].OBJ[idd].Delegate_Insert_Pool.erase(user.Get_id());//原来的授权源删除user的id
									this->OBJ[idd].Delegate_Insert_Pool.insert(user.Get_id());
									user.OBJ[idd].Modify_Source_Insert(this->Get_id());//user的授权源修改为本次授权者
									user.OBJ[idd].Modify_Delegate_Insert(CA);//授权传递标志位取决于本次授权者
									cout << "授权已覆盖" << this->Get_Username() << "->" << user.Get_Username() << endl;
									return 1;
								}
							}
							cout << "权限不足，授权失败[read]" << endl;
						}
					}
				}
			}
			cout << "查找对象OBJ不存在!" << endl;
		}
		else
			cout << "无权授权!" << endl;
		return 0;
	}
	int User::DelegateUpdate(Object& obj, User& user, bool CA) {//默认不允许传递
	//检查自身是否有Obj的权限
		if (Check_Update(obj) && CA_Update(obj)) {//存在授权条件
			for (vector<Object>::iterator it = user.OBJ.begin(); it != user.OBJ.end(); it++) {
				if (obj.Get_Id() == (*it).Get_Id()) {//找到待授权用户的obj的id号
					int idd = distance(user.OBJ.begin(), it);//得到对象在OBJ中的偏移地址
					if (!user.OBJ[idd].Get_Update()) {//之前没有被授权过,判断user的read权限是否为false
						user.OBJ[idd].Modify_Update(true);//得到授权
						user.OBJ[idd].Modify_Source_Update(this->Get_id());//存储授权方的id
						user.OBJ[idd].Modify_Delegate_Update(CA);//存储授权是否允许转让标志
						this->OBJ[idd].Delegate_Update_Pool.insert(user.Get_id());//授权方存储得到授权者的id
						cout << "授权成功!" << this->Get_Username() << "->" << user.Get_Username() << endl;;
						return 1;
					}
					else//read标志位true
					{
						int lid = user.OBJ[idd].Get_Source_Update();//得到直接授权源的id号
						if (lid == this->Get_id()) {
							if (CA!=user.OBJ[idd].Get_Delegate_Update()) {
								user.OBJ[idd].Modify_Delegate_Update(CA);//存储授权是否允许转让标志
								cout << "授权成功!" << endl;
								return 1;
							}
							else
								cout << "已授权，无需重复!" << endl;
							return 0;
						}
						if (usergroup[r[lid]].OBJ[idd].Delegate_Update_Pool.count(this->Get_id())) {
							//直接授权源包含本次授权者，授权无效
							cout << "权限不足,授权失败[read]" << this->Get_Username() << "->" << user.Get_Username() << endl;
							return 0;
						}
						else//不存在直接包含关系，需要向上索引
						{
							int temp = lid;
							while (temp != 0) {
								temp = usergroup[r[temp]].OBJ[idd].Get_Source_Update();//向上访问授权源
								if (temp == this->Get_id()) {//本次授权者权限更高
									usergroup[r[lid]].OBJ[idd].Delegate_Update_Pool.erase(user.Get_id());//原来的授权源删除user的id
									this->OBJ[idd].Delegate_Update_Pool.insert(user.Get_id());
									user.OBJ[idd].Modify_Source_Update(this->Get_id());//user的授权源修改为本次授权者
									user.OBJ[idd].Modify_Delegate_Update(CA);//授权传递标志位取决于本次授权者
									cout << "授权已覆盖" << this->Get_Username() << "->" << user.Get_Username() << endl;
									return 1; 
								}
							}
							cout << "权限不足，授权失败[read]" << endl;
						}
					}
				}
			}
			cout << "查找对象OBJ不存在!" << endl;
		}
		else
			cout << "无权授权!" << endl;
		return 0;
	}
	int User::DelegateDelete(Object& obj, User& user, bool CA) {//默认不允许传递
	//检查自身是否有Obj的权限
		if (Check_Delete(obj) && CA_Delete(obj)) {//存在授权条件
			for (vector<Object>::iterator it = user.OBJ.begin(); it != user.OBJ.end(); it++) {
				if (obj.Get_Id() == (*it).Get_Id()) {//找到待授权用户的obj的id号
					int idd = distance(user.OBJ.begin(), it);//得到对象在OBJ中的偏移地址
					if (!user.OBJ[idd].Get_Delete()) {//之前没有被授权过,判断user的read权限是否为false
						user.OBJ[idd].Modify_Delete(true);//得到授权
						user.OBJ[idd].Modify_Source_Delete(this->Get_id());//存储授权方的id
						user.OBJ[idd].Modify_Delegate_Delete(CA);//存储授权是否允许转让标志
						this->OBJ[idd].Delegate_Delete_Pool.insert(user.Get_id());//授权方存储得到授权者的id
						cout << "授权成功!" << this->Get_Username() << "->" << user.Get_Username() << endl;;
						return 1;
					}
					else//read标志位true
					{
						int lid = user.OBJ[idd].Get_Source_Delete();//得到直接授权源的id号
						if (lid == this->Get_id()) {
							if (CA != user.OBJ[idd].Get_Delegate_Delete()) {
								user.OBJ[idd].Modify_Delegate_Delete(CA);//存储授权是否允许转让标志
								cout << "授权成功!" << endl;
								return 1;
							}
							else
								cout << "已授权，无需重复!" << endl;
							return 0;
						}
						if (usergroup[r[lid]].OBJ[idd].Delegate_Delete_Pool.count(this->Get_id())) {
							//直接授权源包含本次授权者，授权无效
							cout << "权限不足,授权失败[read]" << this->Get_Username() << "->" << user.Get_Username() << endl;
							return 0;
						}
						else//不存在直接包含关系，需要向上索引
						{
							int temp = lid;
							while (temp != 0) {
								temp = usergroup[r[temp]].OBJ[idd].Get_Source_Delete();//向上访问授权源
								if (temp == this->Get_id()) {//本次授权者权限更高
									usergroup[r[lid]].OBJ[idd].Delegate_Delete_Pool.erase(user.Get_id());//原来的授权源删除user的id
									this->OBJ[idd].Delegate_Delete_Pool.insert(user.Get_id());
									user.OBJ[idd].Modify_Source_Delete(this->Get_id());//user的授权源修改为本次授权者
									user.OBJ[idd].Modify_Delegate_Delete(CA);//授权传递标志位取决于本次授权者
									cout << "授权已覆盖" << this->Get_Username() << "->" << user.Get_Username() << endl;
									return 1;
								}
							}
							cout << "权限不足，授权失败[read]" << endl;
						}
					}
				}
			}
			cout << "查找对象OBJ不存在!" << endl;
		}
		else
			cout << "无权授权!" << endl;
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
			cout << "权限不足,权限回收失败" << this->Get_Username() << "->" << user.Get_Username() << endl;
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
			cout << "权限不足,权限回收失败" << this->Get_Username() << "->" << user.Get_Username() << endl;
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
			cout << "权限不足,权限回收失败" << this->Get_id() << "->" << user.Get_id() << endl;
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
			cout << "权限不足,权限回收失败" << this->Get_id() << "->" << user.Get_id() << endl;
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
		if (flag) {//其子集均为空
			int i;
			for (i = 0; i < path.size() - 1; i++) {
				cout << usergroup[r[path[i]]].Get_Username() << "->";
			}
			cout << usergroup[r[path[i]]].Get_Username() << endl;
			return;
		}
		else//字集合不为空
		{
			for (set<int>::iterator it = user.OBJ[id].Delegate_Read_Pool.begin(); it != user.OBJ[id].Delegate_Read_Pool.end(); it++) {
				Read_Chain(usergroup[r[*it]], id, path);
			}
		}
	}
	void Insert_Chain(User& user, int id, vector<int>path) {
		path.push_back(user.Get_id());
		int flag = user.OBJ[id].Delegate_Insert_Pool.empty();
		if (flag) {//其子集均为空
			int i;
			for (i = 0; i < path.size() - 1; i++) {
				cout << usergroup[r[path[i]]].Get_Username() << "->";
			}
			cout << usergroup[r[path[i]]].Get_Username() << endl;
			return;
		}
		else//字集合不为空
		{
			for (set<int>::iterator it = user.OBJ[id].Delegate_Insert_Pool.begin(); it != user.OBJ[id].Delegate_Insert_Pool.end(); it++) {
				Insert_Chain(usergroup[r[*it]], id, path);
			}
		}
	}
	void Update_Chain(User& user, int id, vector<int>path) {
		path.push_back(user.Get_id());
		int flag = user.OBJ[id].Delegate_Update_Pool.empty();
		if (flag) {//其子集均为空
			int i;
			for (i = 0; i < path.size() - 1; i++) {
				cout << usergroup[r[path[i]]].Get_Username() << "->";
			}
			cout << usergroup[r[path[i]]].Get_Username() << endl;
			return;
		}
		else//字集合不为空
		{
			for (set<int>::iterator it = user.OBJ[id].Delegate_Update_Pool.begin(); it != user.OBJ[id].Delegate_Update_Pool.end(); it++) {
				Update_Chain(usergroup[r[*it]], id, path);
			}
		}
	}
	void Delete_Chain(User& user, int id, vector<int>path) {
		path.push_back(user.Get_id());
		int flag = user.OBJ[id].Delegate_Delete_Pool.empty();
		if (flag) {//其子集均为空
			int i;
			for (i = 0; i < path.size() - 1; i++) {
				cout << usergroup[r[path[i]]].Get_Username() << "->";
			}
			cout << usergroup[r[path[i]]].Get_Username() << endl;
			return;
		}
		else//字集合不为空
		{
			for (set<int>::iterator it = user.OBJ[id].Delegate_Delete_Pool.begin(); it != user.OBJ[id].Delegate_Delete_Pool.end(); it++) {
				Delete_Chain(usergroup[r[*it]], id, path);
			}
		}
	}
